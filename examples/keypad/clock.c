#include <libopencm3/cm3/systick.h>

/* System timestamp in milliseconds since boot */
volatile uint32_t millisecs_since_start = 0;

void sys_tick_handler(void) {
	millisecs_since_start++;
}

/*
 * @brief Configure the system tick to interrupt each 1 msec
 */
void system_clock_setup() {
    /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - one interrupt every 1ms*/
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(9000 -1);

    systick_interrupt_enable();

    /* Start counting. */
    systick_counter_enable();    
}

/*
 * @brief Read system timestamp
 */
uint32_t system_clock_get_milis() {
    return millisecs_since_start;
}