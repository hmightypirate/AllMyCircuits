#include "led.h"

uint16_t async_period = 0;
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
      gpio_toggle(LED_PORT, LED_PIN);

      // resetting counter
      num_calls = 0;
    }
  
  num_calls++;
}

/*
 * @brief sets the green led
 *
 */
void set_green_led()
{
  gpio_clear(LED_PORT, LED_PIN);
}

/*
 * @brief clears the green led
 *
 */
void clear_green_led()
{
  gpio_set(LED_PORT, LED_PIN);
}


/*
 * @brief sets the red led
 *
 */
void set_red_led()
{
  gpio_clear(GPIOB, GPIO12);
}

/*
 * @brief clears the red led
 *
 */
void clear_red_led()
{
  gpio_set(GPIOB, GPIO12);
}
