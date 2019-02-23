#ifndef __COMMONS_H
#define __COMMONS_H

#include "jukebox_songs.h"

#define CALIBRATION_SONG SONG_SUPERMAN_ORDER
#define OUT_OF_LINE_SONG SONG_INDIANAJONES_ORDER
#define OUT_OF_BATTERY_SONG SONG_EUROPE_ORDER

/* milliseconds to wait before next execution loop */
#define LOOP_PERIOD 1

/**
 * Maximum PWM period (max velocity)
 */
#define MAX_PWM_VALUE 999
#define MAX_VEL_MOTOR MAX_PWM_VALUE
#define MIN_VEL_MOTOR 0

/*
 * CLI Velocity Step
 */
#define VELOCITY_STEP 10

/* delay before the car starts running in DELAYED_START_STATE */
#define DELAYED_START_WAIT_TIME 5000

/* delays applied when trying to change a pid/vel mapping */
#define AVAILABLE_MAP_CHANGE_TIME 5000

/* Activate/deactivate special functions */
#define ENABLE_TURBO_MODE 1
#define ENABLE_NOOL_MODE 0

// Disturbing pitch to discern normal/turbo states
#define RUNNING_STATE_PITCH 0

/* Wait time in out of line */
#define MAX_RUNNING_RECOVERY_TIME 10 // ms

/* Hysteresis values for changing state whilst running  */
#define SELECT_RUNNING_STATE_USING_ENCODERS 1
#define NORMAL_TO_TURBO_THRESHOLD (SELECT_RUNNING_STATE_USING_ENCODERS ? 2 : 10)
#define TURBO_TO_NORMAL_THRESHOLD (SELECT_RUNNING_STATE_USING_ENCODERS ? 4 : 20)
#define NORMAL_TO_NOOL_THRESHOLD (SELECT_RUNNING_STATE_USING_ENCODERS ? 27 : 200)
#define NOOL_TO_NORMAL_THRESHOLD (SELECT_RUNNING_STATE_USING_ENCODERS ? 20 : 150)

/* Incremental/Decremental target velocity in NORMAL mode */
#define USE_ENCODERS_FOR_INCDEC 1
#define ENABLE_INCDEC_NORMAL_FLAG 0
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

/* anti-wheelie at start variables */
#define ANTIWHEELIE_AT_START 1
#define MAX_ANTIWHEELIE_VELOCITY 350
#define MAX_ANTIWHEELIE_TIME 500

#endif /* __COMMONS_H */
