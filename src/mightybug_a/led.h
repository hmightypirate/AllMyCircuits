#ifndef __LED_H
#define __LED_H

#include "setup.h"
#include <stdio.h>

#define LED_ASYNC_PERIOD 1000

//FIXME: it uses the internal led (make it configurable)

void set_async_period(uint16_t async_calls);
void async_blink();
void set_red_led();
void clear_red_led();
void set_green_led();
void clear_green_led();

#endif /* __LED_H */
