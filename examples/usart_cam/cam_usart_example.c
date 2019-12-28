#include "cam_usart_example.h"

/*
  @brief blinks the internal led of a board as a function of a value passed by the peripheral

 */
void cam_example()
{

  set_led_mode(LED_1, OFF);
  leds_update();

  while(true)
    {
      uint8_t new_data = get_value_data();

      if (new_data == 0)
	{
	  set_led_mode(LED_1, OFF);	  
	}
      else
	{	  
	  set_led_blink_period(LED_1,  75 * new_data);
	  set_led_mode(LED_1, BLINK);
	}
      
      leds_update();
      
    } 
}
