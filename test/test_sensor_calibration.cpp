#include <unity.h>
#include "../src/sensorCalibration.h"

void test_translate_conversion_time_value() {
    TEST_ASSERT_EQUAL_UINT16(140, translateConversionTimeValue(140));
    TEST_ASSERT_EQUAL_UINT16(588, translateConversionTimeValue(588));
    TEST_ASSERT_EQUAL_UINT16(2116, translateConversionTimeValue(2116));
    TEST_ASSERT_EQUAL_UINT16(8244, translateConversionTimeValue(8244));
    TEST_ASSERT_EQUAL_UINT16(0, translateConversionTimeValue(9999));
}

void test_translate_sample_count_value() {
    TEST_ASSERT_EQUAL_UINT16(1, translateSampleCountValue(1));
    TEST_ASSERT_EQUAL_UINT16(64, translateSampleCountValue(64));
    TEST_ASSERT_EQUAL_UINT16(256, translateSampleCountValue(256));
    TEST_ASSERT_EQUAL_UINT16(1024, translateSampleCountValue(1024));
    TEST_ASSERT_EQUAL_UINT16(0, translateSampleCountValue(777));
}

void test_compute_sample_time() {
    float sampleTime = computeSampleTime(2116, 2116, 64);
    TEST_ASSERT_FLOAT_WITHIN(0.000001f, 0.270848f, sampleTime);
}



