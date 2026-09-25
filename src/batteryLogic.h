#pragma once

#include <math.h>

struct BatteryConfig {
    float capacityAh;
    float chargeEfficiencyPercent;
    float minPercent;
    float tailCurrentmA;
    float fullVoltagemV;
    float fullDelayS;
};

inline float clampPercent(float value) {
    if (value < 0.0f) return 0.0f;
    if (value > 100.0f) return 100.0f;
    return value;
}

inline float calculateChargeDeltaAh(float currentA, float seconds) {
    return (currentA * seconds) / 3600.0f;
}

inline float calculateSoCFromDelta(float currentSoc,
                                   float deltaAh,
                                   float capacityAh,
                                   float chargeEfficiencyPercent) {
    const float normalizedEfficiency = clampPercent(chargeEfficiencyPercent) / 100.0f;
    const float effectiveDelta = deltaAh * normalizedEfficiency;
    return clampPercent(currentSoc - (effectiveDelta / capacityAh) * 100.0f);
}

inline bool isBatteryFull(float socPercent, float currentA, float fullVoltagemV, float measuredVoltagemV) {
    const bool voltageReached = measuredVoltagemV >= fullVoltagemV;
    const bool socReached = socPercent >= 100.0f;
    const bool currentLow = fabsf(currentA) < 0.05f;

    return (voltageReached && currentLow) || socReached;
}

inline BatteryConfig makeBatteryConfig(float capacityAh,
                                       float chargeEfficiencyPercent,
                                       float minPercent,
                                       float tailCurrentmA,
                                       float fullVoltagemV,
                                       float fullDelayS) {
    return BatteryConfig{
        capacityAh,
        chargeEfficiencyPercent,
        minPercent,
        tailCurrentmA,
        fullVoltagemV,
        fullDelayS
    };
}