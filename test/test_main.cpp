#include <unity.h>

void test_runtime_loop_runs_in_order_without_config_change();
void test_runtime_loop_applies_config_change_before_other_tasks();

void test_translate_conversion_time_value();
void test_translate_sample_count_value();
void test_compute_sample_time();

void test_battery_parameters_are_converted();
void test_charge_increases_remaining_with_efficiency();
void test_discharge_decreases_remaining_without_charge_efficiency();
void test_remaining_charge_is_clamped();
void test_period_consumption_uses_average_current();

void test_handle_config_change_true_once();
void test_handle_config_change_false();

void test_victron_decodes_hex_byte();
void test_victron_rejects_non_hex();
void test_victron_frame_checksum_validation();

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    UNITY_BEGIN();

    RUN_TEST(test_runtime_loop_runs_in_order_without_config_change);
    RUN_TEST(test_runtime_loop_applies_config_change_before_other_tasks);

    RUN_TEST(test_translate_conversion_time_value);
    RUN_TEST(test_translate_sample_count_value);
    RUN_TEST(test_compute_sample_time);

    RUN_TEST(test_battery_parameters_are_converted);
    RUN_TEST(test_charge_increases_remaining_with_efficiency);
    RUN_TEST(test_discharge_decreases_remaining_without_charge_efficiency);
    RUN_TEST(test_remaining_charge_is_clamped);
    RUN_TEST(test_period_consumption_uses_average_current);

    RUN_TEST(test_handle_config_change_true_once);
    RUN_TEST(test_handle_config_change_false);

    RUN_TEST(test_victron_decodes_hex_byte);
    RUN_TEST(test_victron_rejects_non_hex);
    RUN_TEST(test_victron_frame_checksum_validation);

    return UNITY_END();
}