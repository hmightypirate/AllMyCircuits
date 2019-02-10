#ifndef __TARGET_H
#define __TARGET_H

#include "commons.h"
#include "encoder.h"
#include "motors.h"
#include "pidvelmap.h"
#include <stdint.h>

void update_sequential_readings(int16_t error, int16_t last_error);
void reset_sequential_readings(void);
void update_target_normal(void);
void update_target_normal_with_encoders(void);

#endif /* __TARGET_H */