#include "fsm.h"

static state_e current_state = CALLIBRATION_STATE;

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
      else if (new_event == DELAYED_RUN_START_EVENT)
        {
          current_state = DELAYED_RUN_START_STATE;
        }
    }
}

/*
 * @brief get current state
 */
state_e get_state()
{
  return current_state;
}
