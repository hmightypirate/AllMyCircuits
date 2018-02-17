/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/adc.h>
#include <stdio.h>
#include <unistd.h>

static void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
        rcc_periph_clock_enable(RCC_GPIOC);
        
	/* Enable clocks for GPIO port B (for GPIO_USART3_TX) and USART3. */
	rcc_periph_clock_enable(RCC_USART1);   

        /* Enable ADC Clock */
        rcc_periph_clock_enable(RCC_ADC1);
        
}

static void usart_setup(void)
{
	/* Setup GPIO pin GPIO_USART1_TX. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 38400);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}


static void adc_setup(void)
{
	int i;

        gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_ANALOG, GPIO0);

        gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_ANALOG, GPIO1);
	

	/* Make sure the ADC doesn't run during config. */
	adc_power_off(ADC1);

	/* We configure everything for one single conversion. */
	adc_disable_scan_mode(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	/* We want to read the temperature sensor, so we have to enable it. */
	
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);

	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	for (i = 0; i < 800000; i++)    /* Wait a bit. */
		__asm__("nop");

	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);
}

        
static void gpio_setup(void)
{
	/* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}


static uint16_t read_adc_naiive(uint8_t channel)
{
  uint8_t channel_array[16];
  channel_array[0] = channel;
  adc_set_regular_sequence(ADC1, 1, channel_array);
  adc_start_conversion_direct(ADC1);
  while (!adc_eoc(ADC1));
  uint16_t reg16 = adc_read_regular(ADC1);
  return reg16;
}


int main(void)
{
  int i;

	clock_setup();
	gpio_setup();
	usart_setup();
        adc_setup();

	/* Blink the LED (PC12) on the board with every transmitted byte. */
	while (1) {
		gpio_toggle(GPIOC, GPIO13);	/* LED on/off */
	 
                uint16_t input_adc0 = read_adc_naiive(0);
                uint16_t input_adc1 = read_adc_naiive(1);

                printf("adc0 %d adc1 %d\n", input_adc0, input_adc1);
                
		for (i = 0; i < 800000; i++)	/* Wait a bit. */
			__asm__("nop");
	}

	return 0;
}
