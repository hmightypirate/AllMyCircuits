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

#define SONG_INDIANAJONES_ORDER 1
#define SONG_LEN_INDIANAJONES 55
#define SONG_VEL_INDIANAJONES 250

#define SONG_SUPERMAN_ORDER 2
#define SONG_LEN_SUPERMAN 50
#define SONG_VEL_SUPERMAN 200

uint8_t init_jukebox_songs(void);
uint8_t get_total_songs_number(void);

#endif /* __JUKEBOX_SONGS_H */
