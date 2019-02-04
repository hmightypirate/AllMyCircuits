#include "fsm.h"

static state_e current_state = IDLE_STATE;
static rnstate_e running_state = RUNNING_NORMAL;
uint32_t running_loop_millisecs = 0; //Used for antiwheelie

// Variables for handling target velocity in normal mode
static uint16_t seq_decrease_line_pos = 0;
static uint16_t seq_increase_line_pos = 0;

uint32_t delay_start_ms = 0;

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
 * @brief get the ms the car entered the running state
 * 
 * Used only to check in main if end debug inertia test
 */
uint32_t get_running_ms()
{
  return running_loop_millisecs;
}

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

void update_state(event_e event)
{
  if (current_state == OUT_OF_BATTERY_STATE)
    return;

  switch (event)
  {
  case OUT_OF_BATTERY_EVENT:
    current_state = OUT_OF_BATTERY_STATE;
    break;
  case ALL_SENSORS_IN_LINE_EVENT:
    current_state = IDLE_STATE;
    break;
  case DELAYED_START_TIMEOUT_EVENT:
    // Set the ms at the start of the running state
    running_loop_millisecs = get_millisecs_since_start();
    current_state = RUNNING_STATE;
    break;
  case BUTTON1_PRESSED_EVENT:
    break;
  case BUTTON1_RELEASED_EVENT:
    switch (current_state)
    {
    case CALIBRATION_STATE:
      if (get_calibrated_sensors_count() >= NUM_SENSORS -
                                                MAX_NUM_NOT_CALLIBRATED_SENSORS)
      {
        set_delay_start_time(get_millisecs_since_start());
        current_state = DELAYED_START_STATE;
      }
      break;
    case RUNNING_STATE:
      current_state = IDLE_STATE;
      break;
    case IDLE_STATE:
      current_state = CALIBRATION_STATE;
    default:;
    }
    break;
  case BUTTON2_PRESSED_EVENT:
    if (current_state == CALIBRATION_STATE)
    {
      current_state = INFO_MAP_STATE;
      push_enable_jukebox();
      enable_jukebox();
    }
    else if (current_state == INFO_MAP_STATE)
    {
      current_state = CHANGE_MAP_STATE;
    }
    break;
  case BUTTON2_RELEASED_EVENT:
    break;
  case BUTTON3_PRESSED_EVENT:
    if ((current_state == IDLE_STATE) || (current_state == CALIBRATION_STATE))
    {
      toggle_sound();
    }
    break;
  case BUTTON3_RELEASED_EVENT:
    break;
  case FORCE_CALIBRATION_EVENT:
    current_state = CALIBRATION_STATE;
  case CHANGE_MAP_TIMEOUT_EVENT:
    current_state = INFO_MAP_STATE;
  default:
    current_state = IDLE_STATE;
  }
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

    int32_t next_vel = vel_maps[get_current_pidvel_map()] + step_qty * diff_acc;

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
      set_target_velocity(vel_turbo_maps[get_current_pidvel_map()]);
    }
    else if (running_state == RUNNING_NOOL)
    {
      set_target_velocity(vel_nool_maps[get_current_pidvel_map()]);
    }
  }
}
