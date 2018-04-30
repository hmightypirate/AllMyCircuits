#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "clock.h"
#include "keypad.h"

/* TODO: Give buttons self explanatory names when their function is clearer */
/* Button status variables */
bool bt1;
bool bt2;
bool bt3;

/* Timestamps of the last measured edges (rising or falling) for each button */
uint32_t bt1_edge_ts;
uint32_t bt2_edge_ts;
uint32_t bt3_edge_ts;

/*
 * @brief Configure keypad GPIOs and initialise variables
 */
void keypad_setup() {
  	gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO14);
  	gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO15);
  	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6);
	
	bt1 = false;
	bt2 = false;
	bt3 = false;
	
	bt1_edge_ts = system_clock_get_milis();
	bt2_edge_ts = system_clock_get_milis();
	bt3_edge_ts = system_clock_get_milis();
}

/*
 * @brief Update keypad status in main loop.
 */
void keypad_loop() {
	bool reading;
    uint32_t elapsed_time;
	
    /* Debouncing is applied to each button independently. A timestamp
     * is kept for each button to track the last edge detected. The button
     * status variable is kept stable for at least DEBOUNCE_PERIOD milliseconds.
     */
	reading = gpio_get(GPIOC, GPIO14);
	elapsed_time = system_clock_get_milis() - bt1_edge_ts;
	if ((elapsed_time > DEBOUNCE_PERIOD) && (reading != bt1)) {
		bt1_edge_ts = system_clock_get_milis();
		bt1 = reading;
	}
    
    reading = gpio_get(GPIOC, GPIO15);
    elapsed_time = system_clock_get_milis() - bt2_edge_ts;
	if ((elapsed_time > DEBOUNCE_PERIOD) && (reading != bt2)) {
		bt2_edge_ts = system_clock_get_milis();
		bt2 = reading;
	}
    
    reading = gpio_get(GPIOB, GPIO6);
    elapsed_time = system_clock_get_milis() - bt3_edge_ts;
    if ((elapsed_time > DEBOUNCE_PERIOD) && (reading != bt3)) {
        bt3_edge_ts = system_clock_get_milis();
        bt3 = reading;
    }
}

/*
 * @brief Read the status of BT1 (PC14)
 */
bool keypad_read_bt1() {	
	return bt1;
}

/*
 * @brief Read the status of BT2 (PC15)
 */
bool keypad_read_bt2() {
	return bt2;
}

/*
 * @brief Read the status of BT3 (PB6)
 */
bool keypad_read_bt3() {
	return bt3;
}