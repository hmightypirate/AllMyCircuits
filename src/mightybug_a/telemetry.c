#include "telemetry.h"

// Line position
void print_telemetry(int millis)
{
	if (SHOW_MILLIS)
		printf("%d,", millis % 10);
	if (SHOW_POSITION)
		printf("%+04d,", get_position());
	if (SHOW_K_P)
		printf("%d,", get_kp());
	if (SHOW_K_I)
		printf("%d,", get_ki());
	if (SHOW_K_D)
		printf("%d,", get_kd());
	if (SHOW_P_VALUE)
		printf("%d,", get_proportional());
	if (SHOW_I_VALUE)
		printf("%d,", get_integral());
	if (SHOW_D_VALUE)
		printf("%d,", get_derivative());
	if (SHOW_LEFT_PWM)
		printf("%d,", (int)get_last_left_vel());
	if (SHOW_RIGHT_PWM)
		printf("%d,", (int)get_last_right_vel());
	if (SHOW_LEFT_ENCODER)
		printf("%u,", (unsigned int)get_left_encoder_ticks());
	if (SHOW_RIGHT_ENCODER)
		printf("%u,", (unsigned int)get_right_encoder_ticks());
	if (SHOW_LEFT_ACC)
		printf("%d,", (int)get_left_acc());
	if (SHOW_RIGHT_ACC)
		printf("%d,", (int)get_right_acc());
	if (SHOW_TARGET_VEL)
		printf("%d,", (int)get_target_velocity());
	if (SHOW_SENSOR_INLINE)
	  printf("%u,", (unsigned int)get_inline_change());
	if (SHOW_SYNCHRO_FLAG)
	  printf("%u,", (unsigned int)get_synchro_flag());
	if (SHOW_SYNCHRO_POINTER)
	  printf("%u,",(unsigned int)get_synchro_sector_idx());
	if (SHOW_MAPPING_POINTER)
	  printf("%u,",(unsigned int)get_mapping_pointer_idx());
	if (SHOW_MAPPING_STATE)
	  printf("%u,",(unsigned int)get_mapping_state());
	if (SHOW_SYNCHRO_STATE)
	  printf("%u,",(unsigned int)get_synchro_state());
	if (SHOW_RPM_LEFT)
	  printf("%u,", (unsigned int)get_current_left_rpm());
	if (SHOW_RPM_RIGHT)
	  printf("%u,", (unsigned int)get_current_right_rpm());	
	  
	  
	
	printf("\n");
}
