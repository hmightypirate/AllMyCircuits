#ifndef __CLI_H
#define __CLI_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libopencm3/stm32/usart.h>
#include "pid.h"
#include "fsm.h"
#include "sensors.h"
#include "motors.h"
#include "cron.h"
#include "led.h"
#include "libjukebox.h"
#include "vbatt.h"
#include "encoder.h"
#include "dma.h"
#include "mapping.h"

bool is_command_received(void);
void execute_command(void);

#endif /* __CLI_H */
