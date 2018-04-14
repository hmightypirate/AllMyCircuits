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

bool is_command_received(void);
void execute_command(void);
void usart1_isr(void);
void send_usart(char *message);
void send_message(char *message);
void command_stop(void);
void command_unknown(char *command);
void check_command_car(void);
void check_command_led(void);
void check_module(void);
void clear_head_tail(void);
void clear_command_line(void);
void set_head_tail(char *origin);
void check_command_pid(void);
void check_command_line(void);
void check_command_fsm(void);
void check_command_motor(void);

#endif /* __CLI_H */
