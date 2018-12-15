#ifndef __COMMONS_H
#define __COMMONS_H

#include "jukebox_songs.h"

#define CALLIBRATION_SONG SONG_SUPERMAN_ORDER
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

//#define DELAY(x) for (i=0; i < DELAY_CALLIBRATION_CALLS; i++) __asm__("nop");



#endif /* __COMMONS_H */
