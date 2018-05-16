#include "keypad.h"


bool is_button[NUM_BUTTONS];

/* variable to store that an edge has been detected (true)
 * it will be set to false by external functions (e.g. after some operation has been executed)
 */
bool is_new_edge[NUM_BUTTONS];

/* Timestamps of the last measured edges (rising or falling) for each button */
uint32_t button_edge_ts[NUM_BUTTONS];

uint8_t button_port_array[NUM_BUTTONS];
uint8_t button_pin_array[NUM_BUTTONS];

/*
 * @brief update keypad status in main loop
 */ 
void keypad_loop(uint32_t current_millis)
{
  for (int i=0; i<NUM_BUTTONS; i++)
    {
      /* Debouncing is applied to each button independently. A timestamp               
       * is kept for each button to track the last edge detected. The button           
       * status variable is kept stable for at least DEBOUNCE_PERIOD milliseconds. 
       */
      bool reading;
      uint32_t elapsed_time;

      reading = gpio_get(button_port_array[i],
                         button_pin_array[i]);
      elapsed_time = current_millis - button_edge_ts[i];
      if ((elapsed_time > DEBOUNCE_PERIOD) && (reading != is_button[i]))
        {
          button_edge_ts[i] = current_millis;
          is_button[i] = reading;
          is_new_edge[i] = true;
        }
    }
}

/*
 * @brief sets edge detection of a button to false
 */
void unset_edge(uint8_t id_button)
{
  is_new_edge[id_button] = false;
}

/*
 * @brief initial state of the buttons
 */
void keypad_setup(uint32_t current_millis,
                  uint16_t *port_array,
                  uint16_t *pin_array)
{

  for (int i=0; i<NUM_BUTTONS; i++)
    {
      is_button[i] = false;
      button_edge_ts[i] = current_millis;
      is_new_edge[i] = false;
      
      button_port_array[i] = port_array[i];
      button_pin_array[i] = pin_array[i];
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
  return is_new_edge[id_button];
}

