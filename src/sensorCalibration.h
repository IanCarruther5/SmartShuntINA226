#pragma once

#include <stdint.h>

inline uint16_t translateConversionTimeValue(uint16_t value) {
    switch (value) {
        case 140:   return 140;
        case 204:   return 204;
        case 332:   return 332;
        case 588:   return 588;
        case 1100:  return 1100;
        case 2116:  return 2116;
        case 4156:  return 4156;
        case 8244:  return 8244;
        default:    return 0;
    }
}

inline uint16_t translateSampleCountValue(uint16_t value) {
    switch (value) {
        case 1:    return 1;
        case 4:    return 4;
        case 16:   return 16;
        case 64:   return 64;
        case 128:  return 128;
        case 256:  return 256;
        case 512:  return 512;
        case 1024: return 1024;
        default:   return 0;
    }
}

inline float computeSampleTime(uint16_t conversionTimeShuntUs,
                              uint16_t conversionTimeBusUs,
                              uint16_t samples) {
    return ((float)conversionTimeShuntUs + (float)conversionTimeBusUs) *
           (float)samples *
           0.000001f;
}