#include "led.h"

uint16_t async_period = 0;
uint16_t num_calls = 0;
uint16_t async_period2 = 0;
uint16_t num_calls2 = 0;

/*
 * @brief sets the period of the async blink
 *
 * @param[in] async_calls ticks between toggle calls
 * @param[in] led led number (1 or 2)
 *
 */
void set_async_period(uint16_t async_calls, uint8_t led)
{
  if (led == 1) {
    async_period = async_calls;
  } else if (led == 2) {
    async_period2 = async_calls;
  }
}

/*
 * @brief makes the led blink (using the async period)
 * 
 * @param[in] led led number (1 or 2)
 */
void async_blink(uint8_t led)
{
  if (led == 1) {
  if (num_calls >= async_period)
    {
      gpio_toggle(LED_PORT, LED_PIN);

      // resetting counter
      num_calls = 0;
    }
  num_calls++;
  } else if (led == 2) {
  if (num_calls2 >= async_period2)
    {
      gpio_toggle(LED2_PORT, LED2_PIN);

      // resetting counter
      num_calls2 = 0;
    }
  
  num_calls2++;
  }
}

/*
 * @brief sets the led
 *
 * @param[in] led led number (1 or 2)
 */
void set_led(uint8_t led)
{
  if (led == 1) {
  gpio_clear(LED_PORT, LED_PIN);
  } else if (led == 2) {
    gpio_clear(LED2_PORT, LED2_PIN);    
  }
}

/*
 * @brief clears the led
 *
 * @param[in] led led number (1 or 2)
 */
void clear_led(uint8_t led)
{
  if (led == 1) {
  gpio_set(LED_PORT, LED_PIN);
  } else if (led == 2) {
    gpio_set(LED2_PORT, LED2_PIN);
  }
}

