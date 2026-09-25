#include <unity.h>
#include "../src/batteryLogic.h"

void test_battery_parameters_are_converted() {
    const BatteryParameters params =
        deriveBatteryParameters(100, 95, 10, 500, 14400, 120);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 360000.0f, params.capacityAs);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.95f, params.chargeEfficiency);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.5f, params.tailCurrentA);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 14.4f, params.fullVoltageV);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 36000.0f, params.minAs);
    TEST_ASSERT_EQUAL_UINT32(120000UL, params.fullDelayMs);
}

void test_charge_increases_remaining_with_efficiency() {
    float remainingAs = 1800.0f;
    float consumedAs = 0.0f;

    applyBatteryDelta(remainingAs, consumedAs, 10.0f, 3600.0f, 0.9f);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1809.0f, remainingAs);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 9.0f, consumedAs);
}

void test_discharge_decreases_remaining_without_charge_efficiency() {
    float remainingAs = 1800.0f;
    float consumedAs = 0.0f;

    applyBatteryDelta(remainingAs, consumedAs, -10.0f, 3600.0f, 0.9f);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1790.0f, remainingAs);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -10.0f, consumedAs);
}

void test_remaining_charge_is_clamped() {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, clampRemainingAs(-1.0f, 3600.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3600.0f, clampRemainingAs(4000.0f, 3600.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, clampRemainingAs(100.0f, 0.0f));
}

void test_period_consumption_uses_average_current() {
    const float deltaAs =
        calculatePeriodConsumptionAs(1.0f, 3.0f, 10.0f, 2);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 40.0f, deltaAs);
}

