#include <unity.h>
#include "../src/runtimeLoop.h"

namespace {
int callOrder[7];
int callCount = 0;

void resetOrder() {
    callCount = 0;
}

void recordStep(int step) {
    callOrder[callCount++] = step;
}

void wifi_cb() { recordStep(1); }
void config_change_cb() { recordStep(2); }
void sensor_cb() { recordStep(3); }
void temperature_cb() { recordStep(4); }
void modbus_cb() { recordStep(5); }
void victron_cb() { recordStep(6); }
void display_cb() { recordStep(7); }
}

void test_runtime_loop_runs_in_order_without_config_change() {
    resetOrder();

    bool configChanged = false;
    RuntimeCallbacks callbacks = {
        wifi_cb,
        config_change_cb,
        sensor_cb,
        temperature_cb,
        modbus_cb,
        victron_cb,
        display_cb
    };

    runRuntimeLoop(callbacks, configChanged);

    TEST_ASSERT_EQUAL_INT(6, callCount);
    TEST_ASSERT_EQUAL_INT(1, callOrder[0]);
    TEST_ASSERT_EQUAL_INT(3, callOrder[1]);
    TEST_ASSERT_EQUAL_INT(4, callOrder[2]);
    TEST_ASSERT_EQUAL_INT(5, callOrder[3]);
    TEST_ASSERT_EQUAL_INT(6, callOrder[4]);
    TEST_ASSERT_EQUAL_INT(7, callOrder[5]);
    TEST_ASSERT_FALSE(configChanged);
}

void test_runtime_loop_applies_config_change_before_other_tasks() {
    resetOrder();

    bool configChanged = true;
    RuntimeCallbacks callbacks = {
        wifi_cb,
        config_change_cb,
        sensor_cb,
        temperature_cb,
        modbus_cb,
        victron_cb,
        display_cb
    };

    runRuntimeLoop(callbacks, configChanged);

    TEST_ASSERT_EQUAL_INT(7, callCount);
    TEST_ASSERT_EQUAL_INT(1, callOrder[0]);
    TEST_ASSERT_EQUAL_INT(2, callOrder[1]);
    TEST_ASSERT_EQUAL_INT(3, callOrder[2]);
    TEST_ASSERT_EQUAL_INT(4, callOrder[3]);
    TEST_ASSERT_EQUAL_INT(5, callOrder[4]);
    TEST_ASSERT_EQUAL_INT(6, callOrder[5]);
    TEST_ASSERT_EQUAL_INT(7, callOrder[6]);
    TEST_ASSERT_FALSE(configChanged);
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_runtime_loop_runs_in_order_without_config_change);
    RUN_TEST(test_runtime_loop_applies_config_change_before_other_tasks);
    return UNITY_END();
}