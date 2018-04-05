#include "led.h"

uint16_t async_period = LED_ASYNC_PERIOD;
uint16_t num_calls = 0;

/*
 * @brief sets the calss period of the async blink
 *
 * @param[in] async_calls ticks between toggle calls 
 *
 */
void set_async_period(uint16_t async_calls)
{
  async_period = async_calls;
}

/*
 * @brief makes the led blink (using the async period)
 * 
 */
void async_blink()
{
  if (num_calls >= async_period)
    {
      gpio_toggle(LED_GPIO, LED_PORT);
    }
}

/*
 * @brief sets the led
 *
 */
void set_led()
{
  gpio_set(LED_GPIO, LED_PORT);
}

/*
 * @brief clears the led
 *
 */
void clear_led()
{
  gpio_clear(LED_GPIO, LED_PORT);
}
