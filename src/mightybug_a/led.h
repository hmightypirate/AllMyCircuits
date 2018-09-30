#ifndef __LED_H
#define __LED_H

#include "setup.h"
#include <stdio.h>

#define LED_ASYNC_PERIOD 1000

//FIXME: it uses the internal led (make it configurable)

void set_async_period(uint16_t async_calls, uint8_t led);
void async_blink(uint8_t led);
void set_led(uint8_t led);
void clear_led(uint8_t led);

#endif /* __LED_H */
