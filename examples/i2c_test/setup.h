
#ifndef SETUP_H
#define SETUP_H

#include <stdint.h>

// Returns SETUP_OK if all went well and the following error codes if not
uint8_t setup();
const uint8_t SETUP_OK;
const uint8_t SETUP_ERROR_I2C_COULD_NOT_SETUP;

#endif // SETUP_H
