#ifndef __CLI_H
#define __CLI_H

#include "cron.h"
#include "dma.h"
#include "encoder.h"
#include "fsm.h"
#include "led.h"
#include "libjukebox.h"
#include "mapping.h"
#include "motors.h"
#include "pid.h"
#include "sensors.h"
#include "vbatt.h"
#include <ctype.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_command_received(void);
void execute_command(void);

#endif /* __CLI_H */
