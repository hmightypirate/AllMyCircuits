#ifndef __LED_H
#define __LED_H

#include "setup.h"
#include <stdio.h>

#define LED_ASYNC_PERIOD 1000

//FIXME: it uses the internal led (make it configurable)

void set_async_period(uint16_t async_calls);
void async_blink();
void set_async_period2(uint16_t async_calls);
void async_blink2();
void set_led();
void clear_led();
void set_led2();
void clear_led2();

#endif /* __LED_H */
