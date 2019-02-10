#ifndef __COMMONS_H
#define __COMMONS_H

#include "jukebox_songs.h"

#define CALIBRATION_SONG SONG_SUPERMAN_ORDER
#define OUT_OF_LINE_SONG SONG_INDIANAJONES_ORDER
#define OUT_OF_BATTERY_SONG SONG_EUROPE_ORDER

/* milliseconds to wait before next execution loop */
#define FIXED_LOOP_TIME 1

/**
 * Maximum PWM period (max velocity)
 */
#define MAX_PWM_VALUE 999
#define MAX_VEL_MOTOR 999
#define MIN_VEL_MOTOR 0
#define MAX_INT 65535

/*
 * CLI Velocity Step
 */
#define VELOCITY_STEP 10

/* delay before the car starts running in DELAYED_START_STATE */
#define DELAYED_START_MS 5000

/* delays applied when trying to change a pid/vel mapping */
#define DELAYED_PIDVEL_CHANGE_MS 5000

/* Activate/deactivate special functions */
#define ENABLE_TURBO_MODE 1
#define ENABLE_NOOL_MODE 0

#define TURBO_PITCH_DEBUG 0 // Disturbing pitch to discern normal/turbo states

/* Delay stop if out of line */
#define FLAG_DELAY_STOP_OUT_OF_LINE 1
#define MS_DELAY_OUT_OF_LINE 1600 // ms

/* Hysteresis values for changing state whilst running  */
#define USE_ENCODERS_FOR_STATE 1
#define OUT_NORMAL_HYST USE_ENCODERS_FOR_STATE ? 2 : 10
#define OUT_TURBO_HYST USE_ENCODERS_FOR_STATE ? 4 : 20
#define OUT_NORMAL_NOOL_HYST USE_ENCODERS_FOR_STATE ? 27 : 200
#define OUT_NOOL_NORMAL_HYST USE_ENCODERS_FOR_STATE ? 20 : 150

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

/* anti-wheelie at start variables */
#define FLAG_ANTI_WHEELIE_START 1
#define MAX_VEL_WHEELIE_START 350
#define MAX_DURATION_WHEELIE_START 500

#endif /* __COMMONS_H */
