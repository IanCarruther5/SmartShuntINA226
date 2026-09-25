#include <unity.h>

void test_runtime_loop_runs_in_order_without_config_change();
void test_runtime_loop_applies_config_change_before_other_tasks();
void test_translate_conversion_time_value();
void test_translate_sample_count_value();
void test_compute_sample_time();
void test_calculate_charge_delta_ah();
void test_calculate_soc_from_delta_clamps_range();
void test_is_battery_full();
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
    RUN_TEST(test_calculate_charge_delta_ah);
    RUN_TEST(test_calculate_soc_from_delta_clamps_range);
    RUN_TEST(test_is_battery_full);
    RUN_TEST(test_handle_config_change_true_once);
    RUN_TEST(test_handle_config_change_false);
    RUN_TEST(test_victron_decodes_hex_byte);
    RUN_TEST(test_victron_rejects_non_hex);
    RUN_TEST(test_victron_frame_checksum_validation);
    return UNITY_END();
}