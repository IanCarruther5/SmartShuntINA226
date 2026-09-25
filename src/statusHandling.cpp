#include "common.h"
#include "statusHandling.h"
#include "batteryLogic.h"

BatteryStatus gBattery;


BatteryStatus::BatteryStatus() {
    lastCurrent = 0;
    fullReachedAt = 0;
    lastSoc = 0;
    glidingAverageCurrent = 0;
    lasStatUpdate = 0;
    isSynced = false;
    if (!readStatusFromRTC()) {
        stats.init();
    }
}

void BatteryStatus::setParameters(
    uint16_t capacityAh,
    uint16_t chargeEfficiencyPercent,
    uint16_t minPercent,
    uint16_t tailCurrentmA,
    uint16_t fullVoltagemV,
    uint16_t fullDelayS) {
    const BatteryParameters params = deriveBatteryParameters(
        capacityAh,
        chargeEfficiencyPercent,
        minPercent,
        tailCurrentmA,
        fullVoltagemV,
        fullDelayS);

    batteryCapacity = params.capacityAs;
    chargeEfficiency = params.chargeEfficiency;
    tailCurrent = params.tailCurrentA;
    fullVoltage = params.fullVoltageV;
    minAs = params.minAs;
    fullDelay = params.fullDelayMs;

    // Configuration may have changed while retaining RTC-restored state.
    stats.remainAs = clampRemainingAs(stats.remainAs, batteryCapacity);
    stats.socVal = batterySocFraction(stats.remainAs, batteryCapacity);
}

void BatteryStatus::updateSOC() {
    stats.socVal = batterySocFraction(stats.remainAs, batteryCapacity);

    if (fabs(lastSoc - stats.socVal) >= .005f) {
        writeStatusToRTC();
        lastSoc = stats.socVal;
    }
}

void BatteryStatus::updateTtG() {
    float avgCurrent = getAverageConsumption();
    if (avgCurrent > 0.0) {
        stats.tTgVal = max(stats.remainAs - minAs, 0.0f) / avgCurrent;
    }  else {
        stats.tTgVal = INFINITY;
    }
}

/// @brief 
/// @param current 
/// @param period 
/// @param numPeriods 
void BatteryStatus::updateConsumption(float current, float period,
                                      uint16_t numPeriods) {

    // We use the average between the last and the current value for summation.
    float periodConsumption;

    for (int i = 0; i < numPeriods; ++i) {
        if (currentValues.isFull()) {
          float oldVal;
          currentValues.pop(oldVal);
          glidingAverageCurrent += oldVal;
        }
        currentValues.push(current);
        // Assumtion: Consumption is negative
        glidingAverageCurrent -= current;
    }     

    if(currentValues.isEmpty()) {
        // This is the first measurement, so we don't have an old current value
        lastCurrent = current;
    }
    
    const float rawPeriodConsumption = calculatePeriodConsumptionAs(
        lastCurrent, current, period, numPeriods);

    // Energy statistics use the unadjusted current integration.
    const float energy =
        rawPeriodConsumption / 3.6f / 1000.0f / 10.0f * lastVoltage;

    if (rawPeriodConsumption > 0.0f) {
        stats.amountChargedEnergy += energy;
    } else {
        stats.sumApHDrawn += rawPeriodConsumption / -3.6f;
        stats.amountDischargedEnergy -= energy;
    }

    applyBatteryDelta(
        stats.remainAs,
        stats.consumedAs,
        rawPeriodConsumption,
        batteryCapacity,
        chargeEfficiency);

    lastCurrent = current;
}

float BatteryStatus::getAverageConsumption() {
    uint16_t count = currentValues.size();
    if (count != 0) {
        return  glidingAverageCurrent / count;
    } else {
        return 0;
    }
}
void BatteryStatus::setVoltage(float currVoltage) {
    lastVoltage = currVoltage;
}
void BatteryStatus::setTemperature(float currTemperature) {
    lastTemperature = currTemperature;
}
void BatteryStatus::setHumidity(float currHumidity) {
    lastHumidity = currHumidity;
}
bool BatteryStatus::checkFull() {
    const bool voltageReached = lastVoltage >= fullVoltage - 0.05f;

    if (!voltageReached) {
        fullReachedAt = 0;
        isSynced = false;
        return false;
    }

    if (stats.socVal < 0.90f) {
        // Indicate that the battery is close to full.
        setBatterySoc(0.90f);
    }

    const float current = -getAverageConsumption();
    const bool tailCurrentReached =
        current > 0.0f && current <= tailCurrent;

    if (!tailCurrentReached) {
        fullReachedAt = 0;
        isSynced = false;
        return false;
    }

    // Do not count another sync while the full condition remains true.
    if (isSynced) {
        return false;
    }

    const unsigned long now = millis();
    if (fullReachedAt == 0) {
        fullReachedAt = now;
    }

    if (now - fullReachedAt < fullDelay) {
        return false;
    }

    setBatterySoc(1.0f);
    resetStats();
    isSynced = true;

    stats.secsSinceLastFull = 0;
    stats.numAutoSyncs++;
    stats.lastDischarge = roundf(stats.remainAs / 3.6f);
    stats.consumedAs = 0.0f;

    return true;
}


void BatteryStatus::setBatterySoc(float val) {
    stats.socVal = val;
    stats.remainAs = batteryCapacity * val;
    if(val>=1.0) {
        fullReachedAt = millis();
    }
    updateTtG();
}


void BatteryStatus::resetStats() {
    stats.deepestDischarge = stats.remainAs / 3.6;
}


void BatteryStatus::updateStats(unsigned long now) {
    int timeDeltaSec = (now - lasStatUpdate) / 1000;
    lasStatUpdate = now;
    if (timeDeltaSec < 0) {
        // We had an overflow, so let's assume the last call was 1 sec ago (default interval)
        timeDeltaSec = 1;
    }
    if (stats.secsSinceLastFull >= 0) {
        stats.secsSinceLastFull += timeDeltaSec;
    }


    if (stats.tTgVal != INFINITY) {
        float mAh = stats.consumedAs / 3.6;
        if (stats.deepestDischarge > mAh) {
            stats.deepestDischarge = roundf(mAh);
        }
        
        stats.lastDischarge = roundf(mAh);
        stats.averageDischarge = stats.lastDischarge;
    }

    uint32_t voltageV = lastVoltage * 1000;
    if (stats.minBatVoltage > voltageV) {
        stats.minBatVoltage = voltageV;
    }
    if (stats.maxBatVoltage < voltageV) {
        stats.maxBatVoltage = voltageV;
    }    
}


#ifdef ESP32
RTC_DATA_ATTR Statistics rtcStats;

void BatteryStatus::writeStatusToRTC() {
    memcpy(&rtcStats, &stats, sizeof(stats));
}

bool BatteryStatus::readStatusFromRTC() {
    bool res = true;
    if (rtcStats.magic == MAGICKEY) {
         memcpy(&stats, &rtcStats, sizeof(stats));
    } else {
        res = false;
    }
    return res;
}

#else 
void BatteryStatus::writeStatusToRTC() {
    ESP.rtcUserMemoryWrite(0, (uint32_t*)&stats, sizeof(stats));
}

bool BatteryStatus::readStatusFromRTC() {
    uint32_t magic = 0;
    if (!ESP.rtcUserMemoryRead(0, &magic, sizeof(magic)) || magic != MAGICKEY) {
        return false;
    }

    if (!ESP.rtcUserMemoryRead(0, (uint32_t*)&stats.magic, sizeof(stats))) {
        Serial.println("RTC read failed!");
        return false;
    }

    return true;
}
#endif
