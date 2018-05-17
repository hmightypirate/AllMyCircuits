#include "fsm.h"

static state_e current_state = CALLIBRATION_STATE;

uint32_t delay_start_ms = 0;
uint8_t current_pid_mapping = 0;
uint8_t current_vel_mapping = 0;

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
          current_state = RUNNING_STATE;
        }
      else if (new_event == OUT_OF_BATTERY_EVENT)
        {
          current_state = NO_BATTERY_STATE;
        }
      else if (new_event == GO_TO_DELAYED_START_EVENT)
        {
          current_state = DELAYED_START_STATE;
        }
      else if ((new_event == NEXT_PIDMAP_EVENT) &&
               (current_state == CALLIBRATION_STATE))       
        {
          current_state = PID_MAPPING_STATE;
        }
      else if ((new_event == NEXT_PIDMAP_EVENT) &&
               (current_state == PID_MAPPING_STATE))
        {
          current_state = PID_CHANGE_STATE;
        }
      else if ((new_event == NEXT_VELMAP_EVENT) &&
               (current_state == CALLIBRATION_STATE))
        {
          current_state = VEL_MAPPING_STATE;
        }
      else if ((new_event == NEXT_VELMAP_EVENT) &&
               (current_state == VEL_MAPPING_STATE))
        {
          current_state = VEL_CHANGE_STATE;
        }
    }
}

/*
 * @brief modify the delay start time
 */
void set_delay_start_time(uint32_t delay)
{
  delay_start_ms = delay;
}

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


