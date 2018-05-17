#include "keypad_menu.h"


void menu_functions(uint32_t current_millis)
{
  // Button delayed start (falling edge)
  if (!get_button(BUTTON_START) &&
      get_edge_detection(BUTTON_START))
    {
      set_delay_start_time(current_millis);
      update_state(GO_TO_DELAYED_START_EVENT);
    }

  // Button delayed start: Resetting state (rising edge)
  if (get_button(BUTTON_START) &&
      get_edge_detection(BUTTON_START))
    {
      update_state(FORCE_CALLIBRATION_EVENT);
    }

  // Pid mapping (rising edge)
  if (get_button(BUTTON_PID_MAPPING) &&
      get_edge_detection(BUTTON_PID_MAPPING))
    {
      update_state(NEXT_PIDMAP_EVENT);
    }

  // Vel mapping (rising edge)
  if (get_button(BUTTON_VEL_MAPPING) &&
      get_edge_detection(BUTTON_VEL_MAPPING))
    {
      update_state(NEXT_VELMAP_EVENT);
    }

  // setting all the edges to false
  unset_edge(BUTTON_START);
  unset_edge(BUTTON_PID_MAPPING);
  unset_edge(BUTTON_VEL_MAPPING);
}
