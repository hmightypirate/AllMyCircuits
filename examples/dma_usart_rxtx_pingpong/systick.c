#include "systick.h"

static volatile uint32_t millisecs_since_start = 0;

void sys_tick_handler(void) {
	millisecs_since_start++;
}

uint32_t get_millisecs_since_start() {
	return millisecs_since_start;
}

void systick_setup() {
	// 72MHz / 8 => 9000000 counts per second
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	// 9000000/9000 = 1000 overflows per second - one interrupt each 1ms
	// SysTick interrupt every N clock pulses: set reload to N-1
	systick_set_reload(9000 - 1);

	systick_interrupt_enable();

	systick_counter_enable();
}
