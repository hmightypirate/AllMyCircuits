#ifndef __KEYPAD_MENU_H
#define __KEYPAD_MENU_H

#include <inttypes.h>
#include "setup.h"
#include "keypad.h"
#include "pid.h"
#include "motors.h"
#include "fsm.h"
#include "cron.h"


#define BUTTON_START 0
#define BUTTON_PIDANDVEL_MAPPING 1
#define BUTTON_BUZZER 2


void menu_functions(void);

#endif /* __KEYPAD_MENU_H */
