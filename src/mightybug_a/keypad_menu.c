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
  /*
  if (get_button(BUTTON_START) &&
      get_edge_detection(BUTTON_START))
    {
      update_state(FORCE_CALLIBRATION_EVENT);
    }
  */

  // Pid and vel mapping (rising edge)
  if (get_button(BUTTON_PIDANDVEL_MAPPING) &&
      get_edge_detection(BUTTON_PIDANDVEL_MAPPING))
    {
      set_pidvel_map_time(current_millis);
      update_state(NEXT_PIDANDVELMAP_EVENT);
    }

  // Buzzer on/off mapping (rising edge)
  if (get_button(BUTTON_BUZZER) &&
      get_edge_detection(BUTTON_BUZZER))
    {
      
      update_state(NEXT_BUZZER_EVENT);
    }

  // setting all the edges to false
  unset_edge(BUTTON_START);
  unset_edge(BUTTON_PIDANDVEL_MAPPING);
  unset_edge(BUTTON_BUZZER);
}
