#pragma once

#include <stdint.h>

struct BatteryParameters {
    float capacityAs;
    float chargeEfficiency;
    float tailCurrentA;
    float fullVoltageV;
    float minAs;
    unsigned long fullDelayMs;
};

inline BatteryParameters deriveBatteryParameters(
    uint16_t capacityAh,
    uint16_t chargeEfficiencyPercent,
    uint16_t minPercent,
    uint16_t tailCurrentmA,
    uint16_t fullVoltagemV,
    uint16_t fullDelayS) {
    const float capacityAs = static_cast<float>(capacityAh) * 3600.0f;

    return {
        capacityAs,
        static_cast<float>(chargeEfficiencyPercent) / 100.0f,
        static_cast<float>(tailCurrentmA) / 1000.0f,
        static_cast<float>(fullVoltagemV) / 1000.0f,
        static_cast<float>(minPercent) * capacityAs / 100.0f,
        static_cast<unsigned long>(fullDelayS) * 1000UL
    };
}

inline float calculatePeriodConsumptionAs(
    float lastCurrentA,
    float currentA,
    float periodSeconds,
    uint16_t numPeriods) {
    return ((lastCurrentA + currentA) / 2.0f) *
           periodSeconds *
           static_cast<float>(numPeriods);
}

inline float clampRemainingAs(float remainingAs, float capacityAs) {
    if (capacityAs <= 0.0f || remainingAs < 0.0f) {
        return 0.0f;
    }
    return remainingAs > capacityAs ? capacityAs : remainingAs;
}

// Positive delta is charging; charging efficiency does not reduce discharge.
inline float applyBatteryDelta(
    float& remainingAs,
    float& consumedAs,
    float deltaAs,
    float capacityAs,
    float chargeEfficiency) {
    const float effectiveDelta =
        deltaAs > 0.0f ? deltaAs * chargeEfficiency : deltaAs;

    remainingAs = clampRemainingAs(remainingAs + effectiveDelta, capacityAs);
    consumedAs += effectiveDelta;
    return effectiveDelta;
}

inline float batterySocFraction(float remainingAs, float capacityAs) {
    if (capacityAs <= 0.0f) {
        return 0.0f;
    }
    return clampRemainingAs(remainingAs, capacityAs) / capacityAs;
}