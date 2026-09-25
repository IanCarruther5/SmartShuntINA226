#include <unity.h>
#include "../src/configChange.h"

void test_handle_config_change_true_once() {
    bool paramsChanged = true;

    TEST_ASSERT_TRUE(handleConfigChange(paramsChanged));
    TEST_ASSERT_FALSE(paramsChanged);

    TEST_ASSERT_FALSE(handleConfigChange(paramsChanged));
}

void test_handle_config_change_false() {
    bool paramsChanged = false;

    TEST_ASSERT_FALSE(handleConfigChange(paramsChanged));
    TEST_ASSERT_FALSE(paramsChanged);
}

