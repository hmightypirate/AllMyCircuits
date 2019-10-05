#ifndef __LED_H
#define __LED_H

#include "cron.h"
#include "setup.h"
#include "printf.h"

#define LED_1 0
#define LED_2 1
#define LED_BLINK_PERIOD_DEFAULT 2000

typedef enum {
	OFF,
	ON,
	BLINK,
	DOUBLE_BLINK,
	TRIPLE_BLINK,
	BLINK_ALT,
	DOUBLE_BLINK_ALT
} led_mode_t;

void set_led_blink_period(uint8_t led, uint16_t period);
void leds_update(void);
void set_led_mode(uint8_t led, led_mode_t mode);

#endif /* __LED_H */
