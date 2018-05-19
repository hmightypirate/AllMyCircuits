#include "telemetry.h"

// Line position
void print_telemetry()
{
    if (SHOW_POSITION) printf("%d,", get_position());
    if (SHOW_K_P) printf("%d,", get_kp());
    if (SHOW_K_I) printf("%d,", get_ki());
    if (SHOW_K_D) printf("%d,", get_kd());
    if (SHOW_P_VALUE) printf("%d,", get_proportional());
    if (SHOW_I_VALUE) printf("%d,", get_integral());
    if (SHOW_D_VALUE) printf("%d,", get_derivative());
    if (SHOW_LEFT_PWM) printf("%d,", get_last_left_vel());
    if (SHOW_RIGHT_PWM) printf("%d,", get_last_right_vel());
    if (SHOW_LEFT_ENCODER) printf("%u,", (unsigned int)get_left_velocity());
    if (SHOW_RIGHT_ENCODER) printf("%u,", (unsigned int)get_right_velocity());
    printf("0\n");
}