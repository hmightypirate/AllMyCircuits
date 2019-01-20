#include "fsm.h"

static state_e current_state = IDLE_STATE;
static rnstate_e running_state = RUNNING_NORMAL;
uint32_t running_loop_millisecs = 0; //Used for antiwheelie

// Variables for handling target velocity in normal mode
static uint16_t seq_decrease_line_pos = 0;
static uint16_t seq_increase_line_pos = 0;







const int16_t normal_out_hyst = OUT_NORMAL_HYST; // going out of pid (position)
const int16_t turbo_out_hyst = OUT_TURBO_HYST;   // going out of turbo (position)

const int16_t normal_nool_out_hyst = OUT_NORMAL_NOOL_HYST;
const int16_t nool_normal_out_hyst = OUT_NOOL_NORMAL_HYST;



uint32_t delay_start_ms = 0;
uint32_t pidvel_map_ms = 0;
uint8_t current_pidvel_mapping = INITIAL_PIDVEL_MAPPING;


uint32_t last_ms_inline = 0;


void update_running_state(rnevent_e rnevent)
{
  // loop out of period
  if (rnevent == SET_NORMAL_MODE_STATE)
  {
    set_running_state(RUNNING_NORMAL);
  }
  else if (rnevent == SET_TURBO_MODE_STATE)
  {
    set_running_state(RUNNING_STLINE);
  }
  else if (rnevent == SET_NOOL_MODE_STATE)
  {
    set_running_state(RUNNING_NOOL);
  }
}


/*
 * @brief gets the current ms inline
 */
void update_ms_inline(uint32_t current_ms)
{
  last_ms_inline = current_ms;
}

/*
 * @brief checks if the car has exceeded the time out of line
 */
uint8_t exceeds_time_out_of_line(uint32_t current_ms)
{
  return (current_ms - last_ms_inline) > MS_DELAY_OUT_OF_LINE;
}

/*
 * @brief get the ms the car entered the running state
 * 
 * Used only to check in main if end debug inertia test
 */
uint32_t get_running_ms()
{
  return running_loop_millisecs;
}

/*
 * @brief extremely simple finite state machine
 */
void update_state(event_e new_event)
{
  if (current_state != NO_BATTERY_STATE)
  {
    if (new_event == FORCE_CALLIBRATION_EVENT)
    {
      current_state = CALLIBRATION_STATE;
    }
    else if (new_event == GO_TO_RUN_EVENT)
    {
      if (get_calibrated_sensors_count() >= NUM_SENSORS -
                                                MAX_NUM_NOT_CALLIBRATED_SENSORS)
      {
        current_state = RUNNING_STATE;
        // Set the ms at the start of the running state
        running_loop_millisecs = get_millisecs_since_start();
      }
    }
    else if (new_event == OUT_OF_BATTERY_EVENT)
    {
      current_state = NO_BATTERY_STATE;
    }
    else if (new_event == GO_TO_DELAYED_START_EVENT)
    {
      /* if in callibration -> go to delay start  */
      if (current_state == CALLIBRATION_STATE)
      {
        if (get_calibrated_sensors_count() >= NUM_SENSORS -
                                                  MAX_NUM_NOT_CALLIBRATED_SENSORS)
        {
          current_state = DELAYED_START_STATE;
        }
      }
      /* if running -> go to idle state */
      else if (current_state == RUNNING_STATE)
      {
        current_state = IDLE_STATE;
      }
      /* if in any other state -> go to callibration state */
      else
      {
        current_state = CALLIBRATION_STATE;
      }
    }
    else if ((new_event == NEXT_PIDANDVELMAP_EVENT) &&
             (current_state == CALLIBRATION_STATE))
    {
      push_enable_jukebox();
      enable_jukebox();
      current_state = PIDANDVEL_MAPPING_STATE;
    }
    else if ((new_event == NEXT_PIDANDVELMAP_EVENT) &&
             (current_state == PIDANDVEL_MAPPING_STATE))
    {
      current_state = PIDANDVEL_CHANGE_STATE;
    }
    else if (new_event == FORCE_PIDANDVELMAP_EVENT)
    {
      current_state = PIDANDVEL_MAPPING_STATE;
    }
    else if ((new_event == NEXT_BUZZER_EVENT) &&
             (current_state == CALLIBRATION_STATE))
    {
      if (is_jukebox_enabled())
      {
        disable_jukebox();
      }
      else
      {
        enable_jukebox();
      }
    }
    else if (new_event == FORCE_IDLE_EVENT)
    {
      current_state = IDLE_STATE;
    }
  }
}

/*
 * @brief sets the time entering a pid/vel mapping state
 * 

 */
void set_pidvel_map_time(uint32_t current_time)
{
  pidvel_map_ms = current_time;
}

/*
 * @brief obtain the time entering a pid/vel mapping state
 * 
 * Used to wait x seconds before enable changing pidvel map again
 */
uint32_t get_pidvel_map_time(void)
{
  return pidvel_map_ms;
}

/*
 * @brief modify the delay start time
 * 
 * Used to wait x seconds before running
 */
void set_delay_start_time(uint32_t delay)
{
  delay_start_ms = delay;
}


// Used to wait x seconds before running
uint32_t get_delay_start_time()
{
  return delay_start_ms;
}

/*
 * @brief get current state
 */
state_e get_state()
{
  return current_state;
}

/*
 * @brief set state
 */
void set_state(state_e state)
{
  current_state = state;
}

/*
 * @brief set running state
 */
rnstate_e get_running_state()
{
  return running_state;
}

void set_running_state(rnstate_e state)
{
  running_state = state;
}

/* 
 * @brief obtain next pid/vel mapping
 */
void select_next_pidvel_map()
{
  current_pidvel_mapping = (current_pidvel_mapping + 1) % MAX_MAPPINGS;

  reset_pids_normal();
}

/* 
 * @brief change the pid consts to the normal mapping
*/
void reset_pids_normal()
{
  /* change the pid consts */
  set_kp(pid_maps[current_pidvel_mapping * 3]);
  set_ki(pid_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_maps[current_pidvel_mapping * 3 + 2]);
}

/* 
 * @brief change the pid consts to the turbo mapping
*/
void reset_pids_turbo()
{
  /* change the pid consts */
  set_kp(pid_turbo_maps[current_pidvel_mapping * 3]);
  set_ki(pid_turbo_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_turbo_maps[current_pidvel_mapping * 3 + 2]);
}

/* 
 * @brief change the pid consts to the nool mapping
*/
void reset_pids_nool()
{
  /* change the pid consts */
  set_kp(pid_nool_maps[current_pidvel_mapping * 3]);
  set_ki(pid_nool_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_nool_maps[current_pidvel_mapping * 3 + 2]);
}

/*
 * @brief return the current pid mapping
 */
uint8_t get_current_pidvel_map()
{
  return current_pidvel_mapping;
}

/*
 * @brief get current map song
 * 
 * @note at least must be id map songs + 1
 */
uint8_t get_map_song(uint8_t id_map)
{
  return map_songs[id_map];
}

/*
 * @brief get next sub-state (running)
 *
 */
void get_next_running_state(int16_t avg_proportional)
{

  if (running_state == RUNNING_NORMAL)
  {
    if (ENABLE_TURBO_MODE && (avg_proportional < normal_out_hyst))
    {
      update_running_state(SET_TURBO_MODE_STATE);
    }
    else if (ENABLE_NOOL_MODE && (avg_proportional > normal_nool_out_hyst))
    {
      update_running_state(SET_NOOL_MODE_STATE);
    }
  }
  else if (running_state == RUNNING_STLINE)
  {
    if (avg_proportional > turbo_out_hyst)
    {
      update_running_state(SET_NORMAL_MODE_STATE);
    }
  }
  else if (running_state == RUNNING_NOOL)
  {
    if (avg_proportional < nool_normal_out_hyst)
    {
      update_running_state(SET_NORMAL_MODE_STATE);
    }
  }
}

/* 
 * @brief obtains the aggregate number of pos readings improving/decreasing line position
 *
 */
void update_sequential_readings(int16_t new_proportional, int16_t past_proportional)
{
  if (new_proportional < 0)
  {
    new_proportional = -new_proportional;
  }

  if (past_proportional < 0)
  {
    past_proportional = -past_proportional;
  }

  if (new_proportional > past_proportional)
  {
    seq_increase_line_pos += 1;
    seq_decrease_line_pos = 0;
  }
  else
  {
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
  /* only works in normal mode */
  if (running_state == RUNNING_NORMAL)
  {
    if ((seq_decrease_line_pos > DEC_NORMAL_THRESHOLD) && (get_target_velocity() < MAX_VEL_MOTOR_DEC_MODE))
    {
      set_target_velocity(get_target_velocity() + DEC_NORMAL_QTY);
      if (RESET_DEC_AFTER_SET)
      {
        seq_decrease_line_pos = 0;
      }
    }
    else if ((seq_increase_line_pos > INC_NORMAL_THRESHOLD) && (get_target_velocity() > MIN_VEL_MOTOR_INC_MODE))
    {
      set_target_velocity(get_target_velocity() + INC_NORMAL_QTY);
      if (RESET_INC_AFTER_SET)
      {
        seq_increase_line_pos = 0;
      }
    }
  }
}

/* 
 * @brief updates the target velocity
 *
 */
void update_target_normal_with_encoders()
{

  /* only works in normal mode */
  if (running_state == RUNNING_NORMAL)
  {
    int32_t diff_acc = 0;
    // is left wheel running faster than right wheel
    if (get_left_encoder_ticks() > get_right_encoder_ticks())
    {
      // right wheel is acc faster ?
      diff_acc = get_right_acc() - get_left_acc();
    }
    else
    {
      diff_acc = get_left_acc() - get_right_acc();
    }

    int32_t step_qty = 0;

    if (diff_acc > 0)
    {
      step_qty = STEP_NORMAL_QTY_INC;
    }
    else
    {
      step_qty = STEP_NORMAL_QTY_DEC;
    }

    int32_t next_vel = vel_maps[current_pidvel_mapping] + step_qty * diff_acc;

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
}

/*
 * @brief change velocity to avoid wheelie at start
 */
void set_vel_antiwheelie(uint32_t current_loop_millisecs)
{
  if ((current_loop_millisecs - running_loop_millisecs) < MAX_VEL_WHEELIE_START)
  {
    set_target_velocity(MAX_VEL_WHEELIE_START);
  }
  else
  {
    if (running_state == RUNNING_STLINE)
    {
      set_target_velocity(vel_turbo_maps[current_pidvel_mapping]);
    }
    else if (running_state == RUNNING_NOOL)
    {
      set_target_velocity(vel_nool_maps[current_pidvel_mapping]);
    }
  }
}

/* 
 * @brief resets current target velocity with the turbo velocity
 *
 */
void set_target_as_turbo(void)
{
  set_target_velocity(vel_turbo_maps[current_pidvel_mapping]);
}

/* 
 * @brief resets current target velocity with the velocity in corners
 *
 */
void set_target_as_nool(void)
{
  set_target_velocity(vel_nool_maps[current_pidvel_mapping]);
}

/* 
 * @brief resets current target velocity with the normal velocity
 *
 */
void set_target_as_normal(void)
{
  set_target_velocity(vel_maps[current_pidvel_mapping]);
}
