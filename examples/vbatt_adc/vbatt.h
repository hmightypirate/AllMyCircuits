/*
 * vbatt.h
 *
 *  Created on: 21/04/2018
 *      Author: jorge
 */

#ifndef EXAMPLES_VBATT_ADC_VBATT_H_
#define EXAMPLES_VBATT_ADC_VBATT_H_

#include <stdint.h>


void vbatt_adc_setup();

// returns battery in milivolts
uint16_t read_vbatt();

#endif /* EXAMPLES_VBATT_ADC_VBATT_H_ */
