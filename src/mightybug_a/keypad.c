#include "keypad.h"

bool is_button[NUM_BUTTONS];

/* variable to store that an edge has been detected (true)
 * it will be set to false by external functions (e.g. after some operation has been executed)
 */
bool button_edge_detected[NUM_BUTTONS];

/* Timestamps of the last measured edges (rising or falling) for each button */
uint32_t button_edge_ts[NUM_BUTTONS];

uint32_t button_port_array[NUM_BUTTONS] = {BUTTON0_PORT, BUTTON1_PORT, BUTTON2_PORT};
uint16_t button_pin_array[NUM_BUTTONS] = {BUTTON0_PIN, BUTTON1_PIN, BUTTON2_PIN};

/*
 * @brief update keypad status in main loop
 */
void keypad_loop(void)
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    /* Debouncing is applied to each button independently. A timestamp               
       * is kept for each button to track the last edge detected. The button           
       * status variable is kept stable for at least DEBOUNCE_PERIOD milliseconds. 
       */
    bool reading;
    uint32_t elapsed_time;
    uint32_t current_millis = get_millisecs_since_start();

    reading = gpio_get(button_port_array[i], button_pin_array[i]);
    elapsed_time = current_millis - button_edge_ts[i];

    if ((elapsed_time > DEBOUNCE_PERIOD) && (reading != is_button[i]))
    {
      button_edge_ts[i] = current_millis;
      is_button[i] = reading;
      button_edge_detected[i] = true;
    }
  }
}

/*
 * @brief sets edge detection of a button to false
 */
void clear_edge_detected(uint8_t id_button)
{
  button_edge_detected[id_button] = false;
}

/*
 * @brief initial state of the buttons
 */
void keypad_setup(uint32_t current_millis)
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    is_button[i] = false;
    button_edge_ts[i] = current_millis;
    button_edge_detected[i] = false;
  }
}

/*
 * @brief return the status of button
 */
bool get_button(uint8_t id_button)
{
  return is_button[id_button];
}

/*
 * @brief return the edge detection of a button
 */
bool get_edge_detection(uint8_t id_button)
{
  return button_edge_detected[id_button];
}



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
      update_state(FORCE_CALIBRATION_EVENT);
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
