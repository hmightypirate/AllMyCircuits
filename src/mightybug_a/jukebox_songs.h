#ifndef __JUKEBOX_SONGS_H
#define __JUKEBOX_SONGS_H

#include <stdio.h>
#include "libmusic.h"

struct Song {
	int length;
        int bpm;  /* beats per minute */
	uint8_t *notes_pitch;
	uint8_t *notes_duration;
};

#define JUKEBOX_SONGS_TOTAL_SONGS 3
struct Song jukebox_song_list[JUKEBOX_SONGS_TOTAL_SONGS];

/* Check the example buzzer_jukebox for a more commplete list of songs */

#define SONG_EUROPE_ORDER 0
#define SONG_LEN_EUROPE 97
#define SONG_VEL_EUROPE 140

#define SONG_TEARS_IN_HEAVEN_ORDER 1
#define SONG_LEN_TEARS_IN_HEAVEN 17
#define SONG_VEL_TEARS_IN_HEAVEN 112

#define SONG_LIGHTMYFIRE_ORDER 2
#define SONG_LEN_LIGHTMYFIRE 33
#define SONG_VEL_LIGHTMYFIRE 140


uint8_t init_jukebox_songs(void);
uint8_t get_total_songs_number(void);

#endif /* __JUKEBOX_SONGS_H */
