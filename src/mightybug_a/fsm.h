#ifndef __FSM_H
#define __FSM_H

#include <stdint.h>
#include "libjukebox.h"

typedef enum {
  IDLE_STATE,
  CALIBRATION_STATE,
  RUNNING_STATE,
  OUT_OF_BATTERY_STATE,
  STOP_STATE,
  DELAYED_START_STATE,
  PIDANDVEL_CHANGE_STATE,
  PIDANDVEL_MAPPING_STATE,
  CHANGE_MAP_STATE,
  INFO_MAP_STATE,
  MAX_STATES
} state_e;

typedef enum {
  NO_EVENT,
  FORCE_CALIBRATION_EVENT,
  ALL_SENSORS_IN_LINE_EVENT,
  GO_TO_DELAYED_START_EVENT,
  GO_TO_RUN_EVENT,
  OUT_OF_BATTERY_EVENT,
  NEXT_PIDANDVELMAP_EVENT,
  FORCE_PIDANDVELMAP_EVENT,
  BUTTON1_PRESSED_EVENT,
  BUTTON2_PRESSED_EVENT,
  BUTTON3_PRESSED_EVENT,
  BUTTON1_RELEASED_EVENT,
  BUTTON2_RELEASED_EVENT,
  BUTTON3_RELEASED_EVENT,
  DELAYED_START_TIMEOUT_EVENT,
  CHANGED_MAP_EVENT,
  CHANGE_MAP_TIMEOUT_EVENT,
  MAX_EVENTS
} event_e;

/* Sub-states in the RUNNING State 
   NORMAL: normal operation of the car
   TURBO: in straight lines
   NOOL (nearly out of line): harsh correction

*/
typedef enum {
  RUNNING_NORMAL,
  RUNNING_STLINE,
  RUNNING_NOOL,
  RUNNING_RECOVERY,
  RUNNING_STOP,
  MAX_RUNNING_STATES
} rnstate_e;

typedef enum {
  SET_TURBO_MODE_STATE,
  SET_NORMAL_MODE_STATE,
  SET_NOOL_MODE_STATE
} rnevent_e;


state_e get_state(void);
void set_state(state_e state);
void update_state(event_e new_event);

rnstate_e get_running_state();
void set_running_state(rnstate_e state);
void update_running_state(rnevent_e rnevent);

#endif /* __FSM_H */
