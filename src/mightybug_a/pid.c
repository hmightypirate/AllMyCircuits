#include "pid.h"

static uint32_t k_p = 100; // Proportional constant
static uint32_t k_i = 0;   // Integral constant
static uint32_t k_d = 0;   // Derivative constant

static int64_t integral = 0;
static int32_t derivative = 0;
static int32_t proportional = 0;
static int32_t last_error = 0;

/*
 * @brief resets pid variables
 */
void reset_pid()
{
  last_error = 0;
  proportional = 0;
  derivative = 0;
  integral = 0;
}


int32_t trunc_to_range(int32_t value, int32_t min, int32_t max)
{
  int32_t trunc_value = value;
  
  if (value > max) {
    trunc_value = max;
  } else if (value < min) {
    trunc_value = min;
  }
  
  return trunc_value;
}

/*
 * @brief pid calculation
 *
 * @param[in] proportional current measure of sensors
 */
int32_t pid(int32_t error)
{
  int32_t control;

  proportional = error;
  integral += error;
  integral = trunc_to_range(integral, -MAX_INTEGRAL, MAX_INTEGRAL);
  derivative = error - last_error;

  last_error = error;
  
  control = proportional * k_p + integral * k_i + derivative * k_d;
  control = control/PID_CONTROL_DIVISOR;
  
  return control;
}

void set_kp(int kp)
{
  k_p = (uint32_t)kp;
}

void set_ki(int ki)
{
  k_i = (uint32_t)ki;
}

void set_kd(int kd)
{
  k_d = (uint32_t)kd;
}

int get_kp(void)
{
  return k_p;
}

int get_ki(void)
{
  return k_i;
}

int get_kd(void)
{
  return k_d;
}
