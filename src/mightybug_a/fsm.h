#ifndef __FSM_H
#define __FSM_H

#include <inttypes.h>
#include "sensors.h"
#include "pid.h"
#include "motors.h"
#include "libjukebox.h"

/* delay before the car starts running in DELAYED_START_STATE */
#define DELAYED_START_MS  5000

/* delays applied when trying to change a pid/vel mapping */
#define DELAYED_PIDVEL_CHANGE_MS 5000

/* Number of mappings for pid and vel */
#define MAX_MAPPINGS 3
#define NUMBER_PIDVEL_MAPPINGS 3
#define INITIAL_PIDVEL_MAPPING 0


typedef enum {
  IDLE_STATE,
  CALLIBRATION_STATE,
  RUNNING_STATE,
  SET_TURBO_MODE_STATE,
  SET_NORMAL_MODE_STATE,
  NO_BATTERY_STATE,
  STOP_STATE,
  DELAYED_START_STATE,
  PIDANDVEL_CHANGE_STATE,
  PIDANDVEL_MAPPING_STATE,
  MAX_STATES
} state_e;

typedef enum {
  NO_EVENT,
  FORCE_CALLIBRATION_EVENT,
  FORCE_IDLE_EVENT,
  GO_TO_DELAYED_START_EVENT,
  GO_TO_RUN_EVENT,
  GO_TO_TURBO_EVENT,
  GO_TO_NORMAL_EVENT,
  OUT_OF_BATTERY_EVENT,
  NEXT_PIDANDVELMAP_EVENT,
  FORCE_PIDANDVELMAP_EVENT,
  NEXT_BUZZER_EVENT,
  MAX_EVENTS
} event_e;


state_e get_state(void);
void update_state(event_e new_event);
void set_delay_start_time(uint32_t delay);
uint32_t get_delay_start_time();
void set_pidvel_map_time(uint32_t current_time);
uint32_t get_pidvel_map_time(void);
void select_next_pidvel_map(void);
uint8_t get_current_pidvel_map(void);
uint8_t get_map_song(uint8_t id_map);
void reset_mappings(void);

#endif /* __FSM_H */
