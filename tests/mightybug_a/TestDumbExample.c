#include "uart_putchar.h"
#include "unity.h"
#include "DumbExample.h"

void setUp(void){
	uart_putchar_configure();
	for (int i = 0; i < 1000000000; ++i) __asm__("nop");
}

void tearDown(void){
}

void test_AverageThreeBytes_should_AverageMidRangeValues(void){
	TEST_ASSERT_EQUAL_HEX8(40, AverageThreeBytes(30, 40, 50));
	TEST_ASSERT_EQUAL_HEX8(40, AverageThreeBytes(10, 70, 40));
	TEST_ASSERT_EQUAL_HEX8(33, AverageThreeBytes(33, 33, 33));
}

void test_AverageThreeBytes_should_AverageHighValues(void){
	TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
	TEST_ASSERT_EQUAL_HEX8(127, AverageThreeBytes(127, 127, 127));
	TEST_ASSERT_EQUAL_HEX8(84, AverageThreeBytes(0, 126, 126));
}

int main(void){
	for (int i = 0; i < 100000; ++i) __asm__("nop");
	UNITY_BEGIN();
	RUN_TEST(test_AverageThreeBytes_should_AverageMidRangeValues);
	RUN_TEST(test_AverageThreeBytes_should_AverageHighValues);
	return UNITY_END();
}
