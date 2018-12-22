#include "keypad_menu.h"

bool button_released(uint8_t id_button)
{
  return (get_edge_detection(id_button) && !get_button(id_button));
}

bool button_pressed(uint8_t id_button)
{
  return (get_edge_detection(id_button) && get_button(id_button));
}

void menu_functions(void)
{
  uint32_t millis = get_millisecs_since_start();

  // Button delayed start (falling edge)
  if (button_released(BUTTON_START))
  {
    set_delay_start_time(millis);
    update_state(GO_TO_DELAYED_START_EVENT);
  }

  // Button delayed start: Resetting state (rising edge)
  /*
  if (button_pressed(BUTTON_START))
    {
      update_state(FORCE_CALLIBRATION_EVENT);
    }
  */

  // Pid and vel mapping (rising edge)
  if (button_pressed(BUTTON_PIDANDVEL_MAPPING))
  {
    set_pidvel_map_time(millis);
    update_state(NEXT_PIDANDVELMAP_EVENT);
  }

  // Buzzer on/off mapping (rising edge)
  if (button_pressed(BUTTON_BUZZER))
  {
    update_state(NEXT_BUZZER_EVENT);
  }

  // setting all the edges to false
  clear_edge_detected(BUTTON_START);
  clear_edge_detected(BUTTON_PIDANDVEL_MAPPING);
  clear_edge_detected(BUTTON_BUZZER);
}
