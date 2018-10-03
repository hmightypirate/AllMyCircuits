#include "led.h"

LED_MODE led_mode[2] = {OFF, OFF};
uint16_t led_half_period[2] = {LED_BLINK_PERIOD / 2, LED_BLINK_PERIOD / 2};
uint32_t led_last_toggle[2] = {0, 0};

uint32_t led_port[2] = {LED_PORT, LED2_PORT};
uint32_t led_pin[2] = {LED_PIN, LED2_PIN};

/*
 * @brief sets the period of the async blink
 *
 * @param[in] led led number (1 or 2)
 * @param[in] period time of blink repetition
 *
 */
void set_led_blink_period(uint8_t led, uint16_t period)
{
  if (led > 1) return;
  led_half_period[led] = period / 2;
}

/*
 * @brief sets the led mode (off, on, blink, double_blink, triple_blink)
 *
 * @param[in] led led number (1 or 2)
 * @param[in] mode mode of blink (off, on, blink, double_blink, triple_blink)
 *
 */
void set_led_mode(uint8_t led, LED_MODE mode)
{
  if (led > 1) return;
  led_mode[led] = mode;
}

/*
 * @brief makes the led blink
 * 
 * @param[in] led led number (1 or 2)
 */
void blink_led(uint8_t led, uint32_t millis)
{
  if (millis > (led_last_toggle[led] + led_half_period[led])) {
    gpio_toggle(led_port[led], led_pin[led]);
    led_last_toggle[led] = millis;
  }
}

/*
 * @brief sets the led
 *
 * @param[in] led led number (1 or 2)
 */
void set_led(uint8_t led)
{
  gpio_clear(led_port[led], led_pin[led]);
}

/*
 * @brief clears the led
 *
 * @param[in] led led number (1 or 2)
 */
void clear_led(uint8_t led)
{
  gpio_set(led_port[led], led_pin[led]);
}


/*
 * @brief controls the double blink of led
 *
 * @param[in] led led number (1 or 2)
 */
void double_blink(uint8_t led, uint32_t millis)
{
  if (millis > (led_last_toggle[led]+2*led_half_period[led])) {
    set_led(led);
    led_last_toggle[led] = millis;
    return;
  }

  if (millis > (led_last_toggle[led]+led_half_period[led])) {
    clear_led(led);
    return;
  }

  if (millis > (led_last_toggle[led]+2*led_half_period[led]/3)) {
    set_led(led);
    return;
  }

  if (millis > (led_last_toggle[led]+led_half_period[led]/3)) {
    clear_led(led);
    return;
  }
}


/*
 * @brief controls the triple blink of led
 *
 * @param[in] led led number (1 or 2)
 */
void triple_blink(uint8_t led, uint32_t millis)
{
  if (millis > (led_last_toggle[led]+2*led_half_period[led])) {
    set_led(led);
    led_last_toggle[led] = millis;
    return;
  }

  if (millis > (led_last_toggle[led]+led_half_period[led])) {
    clear_led(led);
    return;
  }

  if (millis > (led_last_toggle[led]+4*led_half_period[led]/5)) {
    set_led(led);
    return;
  }

  if (millis > (led_last_toggle[led]+3*led_half_period[led]/5)) {
    clear_led(led);
    return;
  }

  if (millis > (led_last_toggle[led]+2*led_half_period[led]/5)) {
    set_led(led);
    return;
  }

  if (millis > (led_last_toggle[led]+led_half_period[led]/5)) {
    clear_led(led);
    return;
  }
}


/*
 * @brief update the state of the leds
 *
 * @param[in] millis milliseconds since start
 */
void leds_update(uint32_t millis)
{
  for (uint8_t led = 0; led < 2; led++) {
    switch (led_mode[led]) {
      case OFF:
        clear_led(led);
        break;
      case ON:
        set_led(led);
        break;
      case BLINK:
        blink_led(led, millis);
        break;
      case DOUBLE_BLINK:
        double_blink(led, millis);
        break;
      case TRIPLE_BLINK:
        triple_blink(led, millis);
    }
  }
}