#ifndef __FSM_H
#define __FSM_H

typedef enum {
  CALLIBRATION_STATE,
  RUNNING_STATE,
  NO_BATTERY_STATE,
  STOP_STATE,
  MAX_STATES
} state_e;

typedef enum {
  NO_EVENT,
  FORCE_CALLIBRATION_EVENT,
  GO_TO_RUN_EVENT,
  OUT_OF_BATTERY_EVENT,
  MAX_EVENTS
} event_e;

state_e get_state(void);
void update_state(event_e new_event);  

#endif /* __FSM_H */
