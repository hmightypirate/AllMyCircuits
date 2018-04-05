#ifndef __LED_H
#define __LED_H

#include "setup.h"
#include <stdio.h>

#define LED_ASYNC_PERIOD 100

//FIXME: it uses the internal led (make it configurable)

void set_async_period(uint16_t async_calls);
void async_blink();
void set_led();
void clear_led();

#endif /* __LED_H */
