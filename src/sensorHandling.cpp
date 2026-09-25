#include <Arduino.h>
#include <Wire.h>
#include <INA226.h>

#include "common.h"
#include "sensorHandling.h"
#include "statusHandling.h"
#include "temperature.h"
#include "display.h"
#include "sensorCalibration.h"
#include "batteryLogic.h"

struct Shunt {
  float resistance;
  float maxCurrent;
};


volatile uint16_t alertCounter = 0;
double sampleTime = 0;
bool gSensorInitialized=false;

Shunt PZEM017ShuntData[4] = {
    {0.00075, 100}, {0.0015, 50}, {0.000375, 200}, {0.000250, 300}};

static INA226 ina(Wire);

IRAM_ATTR void alert(void) { ++alertCounter; }

uint16_t translateConversionTime(ina226_shuntConvTime_t time) {
    return translateConversionTimeValue((uint16_t)time);
}

uint16_t translateSampleCount(ina226_averages_t value) {
    return translateSampleCountValue((uint16_t)value);
}

#ifdef DEBUG_SENSOR
void checkConfig() {
    SERIAL_DBG_DBG.print("Mode:                  ");
    switch (ina.getMode()) {
        case INA226_MODE_POWER_DOWN:
        SERIAL_DBG.println("Power-Down");
        break;
        case INA226_MODE_SHUNT_TRIG:
        SERIAL_DBG.println("Shunt Voltage, Triggered");
        break;
        case INA226_MODE_BUS_TRIG:
        SERIAL_DBG.println("Bus Voltage, Triggered");
        break;
        case INA226_MODE_SHUNT_BUS_TRIG:
        SERIAL_DBG.println("Shunt and Bus, Triggered");
        break;
        case INA226_MODE_ADC_OFF:
        SERIAL_DBG.println("ADC Off");
        break;
        case INA226_MODE_SHUNT_CONT:
        SERIAL_DBG.println("Shunt Voltage, Continuous");
        break;
        case INA226_MODE_BUS_CONT:
        SERIAL_DBG.println("Bus Voltage, Continuous");
        break;
        case INA226_MODE_SHUNT_BUS_CONT:
        SERIAL_DBG.println("Shunt and Bus, Continuous");
        break;
        default:
        SERIAL_DBG.println("unknown");
    }

    SERIAL_DBG.print("Samples average:       ");
    switch (ina.getAverages()) {
        case INA226_AVERAGES_1:
        SERIAL_DBG.println("1 sample");
        break;
        case INA226_AVERAGES_4:
        SERIAL_DBG.println("4 samples");
        break;
        case INA226_AVERAGES_16:
        SERIAL_DBG.println("16 samples");
        break;
        case INA226_AVERAGES_64:
        SERIAL_DBG.println("64 samples");
        break;
        case INA226_AVERAGES_128:
        SERIAL_DBG.println("128 samples");
        break;
        case INA226_AVERAGES_256:
        SERIAL_DBG.println("256 samples");
        break;
        case INA226_AVERAGES_512:
        SERIAL_DBG.println("512 samples");
        break;
        case INA226_AVERAGES_1024:
        SERIAL_DBG.println("1024 samples");
        break;
        default:
        SERIAL_DBG.println("unknown");
    }

    SERIAL_DBG.print("Bus conversion time:   ");
    switch (ina.getBusConversionTime()) {
        case INA226_BUS_CONV_TIME_140US:
        SERIAL_DBG.println("140uS");
        break;
        case INA226_BUS_CONV_TIME_204US:
        SERIAL_DBG.println("204uS");
        break;
        case INA226_BUS_CONV_TIME_332US:
        SERIAL_DBG.println("332uS");
        break;
        case INA226_BUS_CONV_TIME_588US:
        SERIAL_DBG.println("558uS");
        break;
        case INA226_BUS_CONV_TIME_1100US:
        SERIAL_DBG.println("1.100ms");
        break;
        case INA226_BUS_CONV_TIME_2116US:
        SERIAL_DBG.println("2.116ms");
        break;
        case INA226_BUS_CONV_TIME_4156US:
        SERIAL_DBG.println("4.156ms");
        break;
        case INA226_BUS_CONV_TIME_8244US:
        SERIAL_DBG.println("8.244ms");
        break;
        default:
        SERIAL_DBG.println("unknown");
    }

    SERIAL_DBG.print("Shunt conversion time: ");
    switch (ina.getShuntConversionTime()) {
        case INA226_SHUNT_CONV_TIME_140US:
        SERIAL_DBG.println("140uS");
        break;
        case INA226_SHUNT_CONV_TIME_204US:
        SERIAL_DBG.println("204uS");
        break;
        case INA226_SHUNT_CONV_TIME_332US:
        SERIAL_DBG.println("332uS");
        break;
        case INA226_SHUNT_CONV_TIME_588US:
        SERIAL_DBG.println("558uS");
        break;
        case INA226_SHUNT_CONV_TIME_1100US:
        SERIAL_DBG.println("1.100ms");
        break;
        case INA226_SHUNT_CONV_TIME_2116US:
        SERIAL_DBG.println("2.116ms");
        break;
        case INA226_SHUNT_CONV_TIME_4156US:
        SERIAL_DBG.println("4.156ms");
        break;
        case INA226_SHUNT_CONV_TIME_8244US:
        SERIAL_DBG.println("8.244ms");
        break;
        default:
        SERIAL_DBG.println("unknown");
    }

    SERIAL_DBG.print("Max possible current:  ");
    SERIAL_DBG.print(ina.getMaxPossibleCurrent());
    SERIAL_DBG.println(" A");

    SERIAL_DBG.print("Max current:           ");
    SERIAL_DBG.print(ina.getMaxCurrent());
    SERIAL_DBG.println(" A");

    SERIAL_DBG.print("Max shunt voltage:     ");
    SERIAL_DBG.print(ina.getMaxShuntVoltage());
    SERIAL_DBG.println(" V");

    SERIAL_DBG.print("Max power:             ");
    SERIAL_DBG.print(ina.getMaxPower());
    SERIAL_DBG.println(" W");

}
#endif

void sensorSetShunt(uint16_t id) {
    if(id < sizeof(PZEM017ShuntData)/ sizeof(Shunt)) {
        gShuntResistancemR = PZEM017ShuntData[id].resistance * 1000.0f;
        gMaxCurrentA = PZEM017ShuntData[id].maxCurrent;

        ina.calibrate(PZEM017ShuntData[id].resistance,PZEM017ShuntData[id].maxCurrent);
    }
    
}

void setupSensor() {
    // Default INA226 address is 0x40
    gSensorInitialized = ina.begin(0x40);

    // Check if the connection was successful, stop if not
    if (!gSensorInitialized) {
        SERIAL_DBG.println("Connection to sensor failed");
        displayMessage("No current sensor found");
    }

    // Configure INA226
    ina.configure(INA226_AVERAGES_64, INA226_BUS_CONV_TIME_2116US,
                 INA226_SHUNT_CONV_TIME_2116US, INA226_MODE_SHUNT_BUS_CONT);
    ina.calibrate(gShuntResistancemR / 1000.0f, gMaxCurrentA);
    ina.enableConversionReadyAlert();

    uint16_t conversionTimeShunt =
        translateConversionTime(ina.getShuntConversionTime());
    uint16_t conversionTimeBus =
        translateConversionTime((ina226_shuntConvTime_t)ina.getBusConversionTime());
    uint16_t samples =
        translateSampleCount(ina.getAverages());

    // This is the time it takes to create a new measurement
    sampleTime = computeSampleTime(conversionTimeShunt, conversionTimeBus, samples);
}

void sensorInit() {
    Wire.begin(PIN_SDA, PIN_SCL);
    attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), alert, FALLING);

    setupSensor();

#ifdef DEBUG_SENSOR
    checkConfig();
#endif

    gBattery.setParameters(
        gCapacityAh,
        gChargeEfficiencyPercent,
        gMinPercent,
        gTailCurrentmA,
        gFullVoltagemV,
        gFullDelayS
    );
}

void updateAhCounter() {
    int count;
    noInterrupts();
    count = alertCounter;
    alertCounter = 0;
    interrupts();

    float current = ina.readShuntCurrent() * gCurrentCalibrationFactor;

    const float deltaAh = calculateChargeDeltaAh(current, sampleTime);
    gBattery.updateConsumption(current, sampleTime, count);

    if (count > 1) {
        SERIAL_DBG.printf("Overflow %d\n",count);
    } 
}

void sensorLoop() {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();

    if(!gSensorInitialized) {
        return;
    }

    if(gParamsChanged) {
        ina.calibrate(gShuntResistancemR / 1000.0, gMaxCurrentA);    
        gBattery.setParameters(gCapacityAh,gChargeEfficiencyPercent,gMinPercent,gTailCurrentmA,gFullVoltagemV,gFullDelayS);
    }

    while (alertCounter && ina.isConversionReady()) {           
        updateAhCounter();
        gBattery.setVoltage(ina.readBusVoltage() * gVoltageCalibrationFactor);
        temperatureLoop();
    }
    
    if (now - lastUpdate >= UPDATE_INTERVAL) {
        gBattery.checkFull();        
        gBattery.updateSOC();        
        gBattery.updateTtG();
        gBattery.updateStats(now);
        lastUpdate = now;
    }
   
}
