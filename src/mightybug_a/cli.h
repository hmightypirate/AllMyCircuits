#ifndef __CLI_H
#define __CLI_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libopencm3/stm32/usart.h>
#include "pid.h"
#include "fsm.h"

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

#endif /* __CLI_H */
