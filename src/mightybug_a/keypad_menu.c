#include "keypad_menu.h"

void menu_functions(uint32_t current_millis)
{
  // Button vel (detects after relasing button
  if (!get_button(BUTTON_START) &&
      get_edge_detection(BUTTON_START))
    {
      set_delay_start_time(current_millis);
      update_state(GO_TO_DELAYED_START_EVENT);
    }

  // setting all the edges to false
  unset_edge(BUTTON_START);
  unset_edge(BUTTON_PID_MAPPING);
  unset_edge(BUTTON_VEL_MAPPING);
}
