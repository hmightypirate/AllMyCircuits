#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "setup.h"

/* Debounce period defined in ms */
#define DEBOUNCE_PERIOD 10

void keypad_setup(uint32_t current_millis,
                  uint16_t *port_array,
                  uint16_t *pin_array);

bool get_button(uint8_t id_button);

#endif /* __KEYPAD_H */
