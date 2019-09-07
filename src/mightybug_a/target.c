#include "target.h"

// Variables for handling target velocity in normal mode
static uint16_t seq_decrease_line_pos = 0;
static uint16_t seq_increase_line_pos = 0;

/*
 * @brief obtains the aggregate number of pos readings improving/decreasing line
 * position
 *
 */
void update_sequential_readings(int16_t error, int16_t last_error)
{
	if (error < 0) {
		error = -error;
	}

	if (last_error < 0) {
		last_error = -last_error;
	}

	if (error > last_error) {
		seq_increase_line_pos += 1;
		seq_decrease_line_pos = 0;
	} else {
		seq_decrease_line_pos += 1;
		seq_increase_line_pos = 0;
	}
}

void reset_sequential_readings(void)
{
	seq_decrease_line_pos = 0;
	seq_increase_line_pos = 0;
}

/*
 * @brief updates the target velocity
 *
 */
void update_target_normal()
{
	if ((seq_decrease_line_pos > DEC_NORMAL_THRESHOLD) &&
	    (get_target_velocity() < MAX_VEL_MOTOR_DEC_MODE)) {
		set_target_velocity(get_target_velocity() + DEC_NORMAL_QTY);
		if (RESET_DEC_AFTER_SET) {
			seq_decrease_line_pos = 0;
		}
	} else if ((seq_increase_line_pos > INC_NORMAL_THRESHOLD) &&
		   (get_target_velocity() > MIN_VEL_MOTOR_INC_MODE)) {
		set_target_velocity(get_target_velocity() + INC_NORMAL_QTY);
		if (RESET_INC_AFTER_SET) {
			seq_increase_line_pos = 0;
		}
	}
}

/*
 * @brief updates the target velocity
 *
 */
void update_target_normal_with_encoders()
{
	int32_t diff_acc = 0;
	// is left wheel running faster than right wheel
	if (get_left_encoder_ticks() > get_right_encoder_ticks()) {
		// right wheel is acc faster ?
		diff_acc = get_right_acc() - get_left_acc();
	} else {
		diff_acc = get_left_acc() - get_right_acc();
	}

	int32_t next_vel = vel_maps[get_current_pidvel_map()];
	
	if (USE_PID_FOR_TARGET_VELOCITY) {
	  int32_t step_qty = pid_target(diff_acc);

	  next_vel =
	    vel_maps[get_current_pidvel_map()] + step_qty;
	  
	} else {
	  int32_t step_qty = 0;

	  if (diff_acc > 0) {
	    step_qty = STEP_NORMAL_QTY_INC;
	  } else {
	    step_qty = STEP_NORMAL_QTY_DEC;
	  }

	  next_vel =
	    vel_maps[get_current_pidvel_map()] + step_qty * diff_acc;
	}	

	// FIXME Check why is this snippet commented?
       	/*
	    if (next_vel < MIN_VEL_MOTOR_INC_MODE)
	      {
		next_vel = MIN_VEL_MOTOR_INC_MODE;
	      }
	    else if (next_vel > MAX_VEL_MOTOR_DEC_MODE)
	      {
		next_vel = MAX_VEL_MOTOR_DEC_MODE;
	      }
	    */

	set_target_velocity(next_vel);
}
