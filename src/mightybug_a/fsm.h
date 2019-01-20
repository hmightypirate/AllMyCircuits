#ifndef __FSM_H
#define __FSM_H

#include <inttypes.h>
#include "sensors.h"
#include "pid.h"
#include "cron.h"
#include "motors.h"
#include "libjukebox.h"
#include "configs.h"

/* delay before the car starts running in DELAYED_START_STATE */
#define DELAYED_START_MS  5000

/* delays applied when trying to change a pid/vel mapping */
#define DELAYED_PIDVEL_CHANGE_MS 5000

/* Activate/deactivate special functions */
#define ENABLE_TURBO_MODE 1

#define ENABLE_NOOL_MODE 0
#define TURBO_PITCH_DEBUG 0  // Disturbing pitch to discern normal/turbo states

/* Delay stop if out of line */
#define FLAG_DELAY_STOP_OUT_OF_LINE 1
#define MS_DELAY_OUT_OF_LINE 1600 // ms

/* Hysteresis values for changing state whilst running  */
#define USE_ENCODERS_FOR_STATE 1
#define OUT_NORMAL_HYST USE_ENCODERS_FOR_STATE? 2:10  
#define OUT_TURBO_HYST USE_ENCODERS_FOR_STATE? 4:20
#define OUT_NORMAL_NOOL_HYST USE_ENCODERS_FOR_STATE? 27:200
#define OUT_NOOL_NORMAL_HYST USE_ENCODERS_FOR_STATE? 20:150


/* Incremental/Decremental target velocity in NORMAL mode */
#define USE_ENCODERS_FOR_INCDEC 1
#define ENABLE_INCDEC_NORMAL_FLAG 1
#define ITS_INCDEC_NORMAL 10
#define INC_NORMAL_THRESHOLD 10
#define DEC_NORMAL_THRESHOLD 10
#define RESET_INC_AFTER_SET 0
#define RESET_DEC_AFTER_SET 0
#define INC_NORMAL_QTY -5
#define MIN_VEL_MOTOR_INC_MODE 250
#define MAX_VEL_MOTOR_DEC_MODE 950
#define DEC_NORMAL_QTY 5
#define STEP_NORMAL_QTY_DEC 40
#define STEP_NORMAL_QTY_INC 8

/* Pickle configuration */
#define TURBO_PICKLE 1
#define TURBO_PICKLE_IN_CORNERS 0
#define PICKLE_ENC_DISTANCE_DOWN 700  // No pickle going down
#define PICKLE_ENC_DISTANCE_UP 3
#define PICKLE_TURBO_VEL 200



/* anti-wheelie at start variables */
#define FLAG_ANTI_WHEELIE_START 1
#define MAX_VEL_WHEELIE_START 350
#define MAX_DURATION_WHEELIE_START 500


typedef enum {
  IDLE_STATE,
  CALLIBRATION_STATE,
  RUNNING_STATE,
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
  OUT_OF_BATTERY_EVENT,
  NEXT_PIDANDVELMAP_EVENT,
  FORCE_PIDANDVELMAP_EVENT,
  NEXT_BUZZER_EVENT,
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
void set_delay_start_time(uint32_t delay);
uint32_t get_delay_start_time();
void set_pidvel_map_time(uint32_t current_time);
uint32_t get_pidvel_map_time(void);
void select_next_pidvel_map(void);
uint8_t get_current_pidvel_map(void);
uint8_t get_map_song(uint8_t id_map);
void reset_pids_normal(void);
void reset_pids_turbo(void);
void reset_pids_nool();
void get_next_running_state(int16_t avg_proportional);
rnstate_e get_running_state();
uint32_t get_running_ms();
void set_running_state(rnstate_e state);
void update_ms_inline(uint32_t current_ms);
uint8_t exceeds_time_out_of_line(uint32_t current_ms);
void update_sequential_readings(int16_t new_proportional, int16_t past_proportional);
void reset_sequential_readings(void);
void update_target_normal();
void update_target_normal_with_encoders();

void set_vel_antiwheelie(uint32_t current_loop_millisecs);

/* Antipickle functions */
void reset_veldelay(void);
int32_t get_next_constrained_target_velocity(int32_t vel);
void increase_pointer_vel_delay(int32_t last_vel);

/* Target velocity modes */
void set_target_as_turbo(void);
void set_target_as_normal(void);
void set_target_as_nool(void);

#endif /* __FSM_H */
