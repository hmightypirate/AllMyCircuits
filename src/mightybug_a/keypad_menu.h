#ifndef __KEYPAD_MENU_H
#define __KEYPAD_MENU_H

#include <inttypes.h>
#include "setup.h"
#include "keypad.h"
#include "pid.h"
#include "motors.h"
#include "fsm.h"


#define BUTTON_START 0
#define BUTTON_PID_MAPPING 1
#define BUTTON_VEL_MAPPING 2


void menu_functions(uint32_t current_millis);

#endif /* __KEYPAD_MENU_H */
