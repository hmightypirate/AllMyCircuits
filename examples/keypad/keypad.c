#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "clock.h"

// Debounce period defined in ms
#define DEBOUNCE_PERIOD 10

/* TODO: Give buttons self explanatory names when their function is clearer */
// Button state variables
bool bt1;
bool bt2;
bool bt3;

// Timestamps of the last measured edges (rising or falling) for each button
uint32_t bt1_edge_ts;
uint32_t bt2_edge_ts;
uint32_t bt3_edge_ts;

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

void keypad_loop() {
	bool reading;
    uint32_t elapsed_time;
	
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

/* TODO: Give buttons self explanatory names when their function is clearer */
bool keypad_read_bt1() {	
	return bt1;
}

/* TODO: Give buttons self explanatory names when their function is clearer */
bool keypad_read_bt2() {
	return bt2;
}

/* TODO: Give buttons self explanatory names when their function is clearer */
bool keypad_read_bt3() {
	return bt3;
}