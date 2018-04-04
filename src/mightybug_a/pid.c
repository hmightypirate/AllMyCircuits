#include "pid.h"

/*
  PID values
  k_p = (0.3 * 100)
  k_i = 0
  k_d = 10 (* 100)

 */
static uint32_t k_p = 30;  /* 0.3 * 100 */
static uint32_t k_i = 0; /* integral part */
static uint32_t k_d = 1000; /* derivative term */ 

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
  else if (integral < 0 && integral < MAX_INTEGRAL)
    {
      integral = -MAX_INTEGRAL;
    }

  derivative = proportional - former_proportional;
  former_proportional = proportional;

  error = proportional * k_p + integral * k_i + derivative * k_d;
  error = error/100;
  
  return error;
}
