#include "led.h"

LED_MODE led_mode[2] = {OFF, OFF};
uint16_t led_half_period[2] = {LED_BLINK_PERIOD / 2, LED_BLINK_PERIOD / 2};
uint32_t led_last_toggle[2] = {0, 0};

/*
 * @brief sets the period of the async blink
 *
 * @param[in] async_calls ticks between toggle calls
 * @param[in] led led number (1 or 2)
 *
 */
void set_led_blink_period(uint8_t led, uint16_t period)
{
  if (led > 2) return;
  led_half_period[led-1] = period;
}

/*
 * @brief makes the led blink
 * 
 * @param[in] led led number (1 or 2)
 */
void blink_led(uint8_t led, uint32_t millis)
{
  if (millis > (led_last_toggle[led-1] + led_half_period[led-1])) {
    if (led == 1) gpio_toggle(LED_PORT, LED_PIN);
    else if (led == 2) gpio_toggle(LED2_PORT, LED2_PIN);
    led_last_toggle[led-1] = millis;
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


void leds_update(uint32_t millis)
{
  for (uint8_t led = 1; led < 3; led++) {
    switch (led_mode[led-1]) {
      case OFF:
        clear_led(led);
        break;
      case ON:
        set_led(led);
        break;
      case BLINK:
        blink_led(led, millis);
        break;
    }
  }
}