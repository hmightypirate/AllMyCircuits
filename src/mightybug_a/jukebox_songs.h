#ifndef __JUKEBOX_SONGS_H
#define __JUKEBOX_SONGS_H

#include "libmusic.h"
#include "printf.h"

struct Song {
	int length;
	int bpm; /* beats per minute */
	uint8_t *notes_pitch;
	uint8_t *notes_duration;
};

#define JUKEBOX_SONGS_TOTAL_SONGS 9

struct Song jukebox_song_list[JUKEBOX_SONGS_TOTAL_SONGS];

/* Check the example buzzer_jukebox for a more commplete list of songs */
#define NO_SONG -1

#define SONG_EUROPE_ORDER 0
#define SONG_LEN_EUROPE 97
#define SONG_VEL_EUROPE 140

#define SONG_INDIANAJONES_ORDER 1
#define SONG_LEN_INDIANAJONES 55
#define SONG_VEL_INDIANAJONES 250

#define SONG_SUPERMAN_ORDER 2
#define SONG_LEN_SUPERMAN 53
#define SONG_VEL_SUPERMAN 190

#define SONG_ONE_BEAT_ORDER 3
#define SONG_LEN_ONE_BEAT 2
#define SONG_VEL_ONE_BEAT 150

#define SONG_TWO_BEAT_ORDER 4
#define SONG_LEN_TWO_BEAT 4
#define SONG_VEL_TWO_BEAT 250

#define SONG_THREE_BEAT_ORDER 5
#define SONG_LEN_THREE_BEAT 6
#define SONG_VEL_THREE_BEAT 250

#define SOPRANO_BEAT_ORDER 6
#define SOPRANO_LEN_BEAT 2
#define SOPRANO_VEL_BEAT 250

#define TENOR_BEAT_ORDER 7
#define TENOR_LEN_BEAT 2
#define TENOR_VEL_BEAT 150

#define BASS_BEAT_ORDER 8
#define BASS_LEN_BEAT 2
#define BASS_VEL_BEAT 50

uint8_t init_jukebox_songs(void);
uint8_t get_total_songs_number(void);

#endif /* __JUKEBOX_SONGS_H */
