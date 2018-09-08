#ifndef __FSM_H
#define __FSM_H

#include <inttypes.h>
#include "sensors.h"
#include "pid.h"
#include "cron.h"
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

/* Activate/deactivate special functions */
#define ENABLE_TURBO_MODE 1
#define TURBO_PITCH_DEBUG 1  // Disturbing pitch to discern normal/turbo states
#define MAX_ITS_CORNER 10
#define MIN_ITS_TURBO 10

/* Delay stop if out of line */
#define FLAG_DELAY_STOP_OUT_OF_LINE 1
#define MS_DELAY_OUT_OF_LINE 600 //ms

#define OUT_NORMAL_HYST 15
#define OUT_TURBO_HYST 25

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

typedef enum {
  RUNNING_NORMAL,
  RUNNING_STLINE,
  MAX_RUNNING_STATES,
} rnstate_e;


state_e get_state(void);
void set_state(state_e state);
void update_state(event_e new_event);
void set_delay_start_time(uint32_t delay);
uint32_t get_delay_start_time();
void set_pidvel_map_time(uint32_t current_time);
uint32_t get_pidvel_map_time(void);
void select_next_pidvel_map(void);
uint8_t get_current_pidvel_map(void);
uint8_t get_map_song(uint8_t id_map);
void force_mapping_to_current(void);
void reset_pids_normal(void);
void reset_pids_turbo(void);
void reset_pids_incorner(void);
void get_next_running_state(int16_t avg_proportional);
rnstate_e get_running_state();
uint32_t get_running_ms();
void set_running_state(rnstate_e state);
void update_ms_inline(uint32_t current_ms);
uint8_t exceeds_time_out_of_line(uint32_t current_ms);

#endif /* __FSM_H */
