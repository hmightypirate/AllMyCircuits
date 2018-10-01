#include "led.h"

uint16_t async_period = 0;
uint16_t num_calls = 0;
uint16_t async_period2 = 0;
uint16_t num_calls2 = 0;

LED_MODE led_mode[2] = {OFF, OFF};
uint16_t led_half_period[2] = {LED_ASYNC_PERIOD / 2, LED_ASYNC_PERIOD / 2};
uint16_t led_last_toggle[2] = {0, 0};

void leds_update(uint16_t millis)
{
  for (uint8_t led_index = 0; led_index < 2; led_index++) {
    switch (led_mode[led_index]) {
      case OFF:
        clear_led(led_index + 1);
        printf("OFF %i ", led_index + 1);
        break;
      case ON:
        set_led(led_index + 1);
        printf("ON %i ", led_index + 1);
        break;
      case BLINK:
      default:
        async_blink(led_index + 1);
        printf("BLINK %i ", led_index + 1);
    }
  }
  printf("\n");
}

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
    if (num_calls >= async_period) {
      gpio_toggle(LED_PORT, LED_PIN);

      // resetting counter
      num_calls = 0;
    }
    num_calls++;
  } else if (led == 2) {
    if (num_calls2 >= async_period2) {
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

void set_led_mode(uint8_t led, LED_MODE mode)
{
  if (led > 2) return;
  led_mode[led - 1] = mode;
}