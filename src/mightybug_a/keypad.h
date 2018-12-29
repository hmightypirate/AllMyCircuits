#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "setup.h"
#include "cron.h"
#include "fsm.h"

/* Debounce period defined in ms */
#define DEBOUNCE_PERIOD 10

void keypad_loop(void);
void keypad_setup(uint32_t current_millis);
bool get_button(uint8_t id_button);
void clear_edge_detected(uint8_t id_button);
bool get_edge_detection(uint8_t id_button);

#define BUTTON_START 0
#define BUTTON_PIDANDVEL_MAPPING 1
#define BUTTON_BUZZER 2

void menu_functions(void);


#endif /* __KEYPAD_H */
