#ifndef __COMMONS_H
#define __COMMONS_H

#include "jukebox_songs.h"

#define CALLIBRATION_SONG SONG_EUROPE_ORDER
#define OUT_OF_LINE_SONG SONG_TEARS_IN_HEAVEN_ORDER
#define OUT_OF_BATTERY SONG_LIGHTMYFIRE_ORDER


/** 
 * Maximum PWM period (max velocity)
*/
#define MAX_VEL_MOTOR 1024
#define INITIAL_TARGET_VELOCITY 200

//#define DELAY(x) for (i=0; i < DELAY_CALLIBRATION_CALLS; i++) __asm__("nop");



#endif /* __COMMONS_H */
