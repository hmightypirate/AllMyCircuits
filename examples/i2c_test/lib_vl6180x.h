
#ifndef LIB_VL6180X_H_
#define LIB_VL6180X_H_

#include <stdint.h>


// returns -1 if could not be initialized
uint8_t VL6180X_setup();

void VL6180X_start_single_range();

void VL6180X_start_continuous_range();

// poll the VL6180X till new sample ready
void VL6180X_poll_range();

 // read range result
uint8_t VL6180X_read_range();

// clear the interrupt on VL6180X
void VL6180X_clear_interrupts();

#endif /* LIB_VL6180X_H_ */
