#ifndef __LED_H
#define __LED_H

#include "setup.h"
#include <stdio.h>

#define LED_ASYNC_PERIOD 2000

typedef enum {OFF, ON, BLINK} LED_MODE;

//FIXME: it uses the internal led (make it configurable)

void set_async_period(uint16_t async_calls, uint8_t led);
void async_blink(uint8_t led);
void set_led(uint8_t led);
void clear_led(uint8_t led);
void leds_update(uint16_t millis);
void set_led_mode(uint8_t led, LED_MODE mode);

#endif /* __LED_H */
