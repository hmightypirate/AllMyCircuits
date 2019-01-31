#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "setup.h"
#include "cron.h"
#include "fsm.h"

/* Debounce period defined in ms */
#define DEBOUNCE_PERIOD 10

void keypad_update(void);
void keypad_setup(uint32_t current_millis);
bool get_button(uint8_t id_button);
bool get_edge_detection(uint8_t id_button);
bool button_pressed(uint8_t id_button);
bool button_released(uint8_t id_button);

#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2

#endif /* __KEYPAD_H */
