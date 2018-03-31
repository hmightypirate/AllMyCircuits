#include "pid.h"


int pid(int proportional)
{
  int error;
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
