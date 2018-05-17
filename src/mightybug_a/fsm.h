#ifndef __FSM_H
#define __FSM_H

#include <inttypes.h>

/* delay before the car starts running in DELAYED_START_STATE */
#define DELAYED_START_MS  5000 

typedef enum {
  CALLIBRATION_STATE,
  RUNNING_STATE,
  NO_BATTERY_STATE,
  STOP_STATE,
  DELAYED_START_STATE,
  PID_MAPPING_STATE,
  PID_CHANGE_STATE,
  VEL_MAPPING_STATE,
  VEL_CHANGE_STATE,
  MAX_STATES
} state_e;

typedef enum {
  NO_EVENT,
  FORCE_CALLIBRATION_EVENT,
  GO_TO_DELAYED_START_EVENT,
  GO_TO_RUN_EVENT,
  OUT_OF_BATTERY_EVENT,
  NEXT_PIDMAP_EVENT,
  NEXT_VELMAP_EVENT,
  MAX_EVENTS
} event_e;

state_e get_state(void);
void update_state(event_e new_event);
void set_delay_start_time(uint32_t delay);
uint32_t get_delay_start_time();

#endif /* __FSM_H */
