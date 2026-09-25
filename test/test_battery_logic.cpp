#include <unity.h>
#include "../src/batteryLogic.h"

void test_calculate_charge_delta_ah() {
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.25f, calculateChargeDeltaAh(1.0f, 900.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.5f, calculateChargeDeltaAh(2.0f, 900.0f));
}

void test_calculate_soc_from_delta_clamps_range() {
    // Positive delta reduces SoC; negative delta increases it.
    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 0.0f,
        calculateSoCFromDelta(50.0f, 10.0f, 10.0f, 100.0f));

    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 100.0f,
        calculateSoCFromDelta(50.0f, -10.0f, 10.0f, 100.0f));
}

void test_is_battery_full() {
    TEST_ASSERT_TRUE(isBatteryFull(99.0f, 0.01f, 5400.0f, 5405.0f));
    TEST_ASSERT_TRUE(isBatteryFull(100.0f, 1.0f, 5400.0f, 5000.0f));
    TEST_ASSERT_FALSE(isBatteryFull(50.0f, 0.2f, 5400.0f, 5000.0f));
}

