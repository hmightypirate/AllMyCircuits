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
#include <stdlib.h>
#include <string.h>

#include "printf.h"

typedef enum {
	UINT8_T,
	UINT16_T,
	UINT32_T,
	INT8_T,
	INT16_T,
	INT32_T
} type_t;

bool is_command_received(void);
void execute_command(void);

void add_config_var(void *ptr, uint8_t id, type_t type, uint8_t lenght);

#endif /* __CLI_H */
