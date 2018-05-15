#include "keypad.h"


bool is_button[NUM_BUTTONS];

/* Timestamps of the last measured edges (rising or falling) for each button */
uint32_t button_edge_ts[NUM_BUTTONS];

/*
 * @brief initial state of the buttons
 */
void keypad_setup(uint32_t current_millis)
{

  for (int i=0; i<NUM_BUTTONS; i++)
    {
      is_button[i] = false;
      button_edge_ts[i] = current_millis;
    }  
}

/*
 * @brief return the status of button
 */
bool get_button(uint8_t id_button)
{
  return is_button[id_button];
}
