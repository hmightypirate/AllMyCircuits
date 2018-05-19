#include "telemetry.h"

// Line position
void print_telemetry()
{
    if (SHOW_POSITION) printf("%d,", get_position());
    if (SHOW_K_P) printf("%d,", get_kp());
    if (SHOW_K_I) printf("%d,", get_ki());
    if (SHOW_K_D) printf("%d,", get_kd());
    if (SHOW_LEFT_PWM) printf("%d,", get_last_left_vel());
    if (SHOW_RIGHT_PWM) printf("%d,", get_last_right_vel());
    if (SHOW_LEFT_ENCODER) printf("%d,", get_left_velocity());
    if (SHOW_RIGHT_ENCODER) printf("%d,", get_right_velocity());
    printf("\b\n");
}