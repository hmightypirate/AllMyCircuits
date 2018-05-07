#include "pid.h"

static uint32_t k_p = 100;  /* 0.3 * 100 */
static uint32_t k_i = 0; /* integral part */
static uint32_t k_d = 0; /* derivative term */ 

static int64_t integral = 0;
static int32_t derivative = 0;
static int32_t former_proportional = 0;

/*
 * @brief resets pid variables
 */
void reset_pid()
{
  former_proportional = 0;
  derivative = 0;
  integral = 0;
}

/*
 * @brief pid calculation
 *
 * @param[in] proportional current measure of sensors
 */
int32_t pid(int32_t proportional)
{
  int32_t error;
  integral = integral + proportional;
  if (integral > 0 && integral > MAX_INTEGRAL)
    {
      integral = MAX_INTEGRAL;
    }
  else if (integral < 0 && integral < -MAX_INTEGRAL)
    {
      integral = -MAX_INTEGRAL;
    }

  derivative = proportional - former_proportional;
  former_proportional = proportional;

  error = proportional * k_p + integral * k_i + derivative * k_d;
  error = error/PID_ERROR_DIVISOR;
  
  return error;
}


void set_kp(int kp) {
  k_p = (uint32_t)kp;
}

void set_ki(int ki) {
  k_i = (uint32_t)ki;
}

void set_kd(int kd) {
  k_d = (uint32_t)kd;
}

int get_kp(void) {
  return k_p;
}

int get_ki(void) {
  return k_i;
}

int get_kd(void) {
  return k_d;
}
