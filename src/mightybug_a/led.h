#ifndef __LED_H
#define __LED_H

#include "setup.h"
#include <stdio.h>
#include "cron.h"

#define LED1 0
#define LED2 1
#define LED_BLINK_PERIOD 2000

typedef enum {OFF, ON, BLINK, DOUBLE_BLINK, TRIPLE_BLINK, BLINK_ALT, DOUBLE_BLINK_ALT} LED_MODE;

//FIXME: it uses the internal led (make it configurable)

void set_led_blink_period(uint8_t led, uint16_t period);
void leds_update(void);
void set_led_mode(uint8_t led, LED_MODE mode);

#endif /* __LED_H */
