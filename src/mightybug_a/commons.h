#ifndef __COMMONS_H
#define __COMMONS_H

#include "jukebox_songs.h"

#define CALLIBRATION_SONG SONG_SUPERMAN_ORDER
#define OUT_OF_LINE_SONG SONG_INDIANAJONES_ORDER
#define OUT_OF_BATTERY_SONG SONG_EUROPE_ORDER

/* ticks to wait in (systick ticks) before executing a reading/pid 
   execution)
*/
#define TIME_BETWEEN_LOOP_ITS 1

/** 
 * Maximum PWM period (max velocity)
*/
#define MAX_PWM_VALUE 1023
#define MAX_VEL_MOTOR 999
#define MIN_VEL_MOTOR 0
#define MAX_INT 65535


/*
 * CLI Velocity Step
 */ 
#define VELOCITY_STEP 10

//#define DELAY(x) for (i=0; i < DELAY_CALLIBRATION_CALLS; i++) __asm__("nop");



#endif /* __COMMONS_H */
