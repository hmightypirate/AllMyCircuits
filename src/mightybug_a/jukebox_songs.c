#include "jukebox_songs.h"

static uint8_t notes_pitch_SUPERMAN[SONG_LEN_SUPERMAN] = {
        D5, D5, D5, G5, R, G5, D6, R, D6, E6, D6, C6, D6, R, D5, D5, D5, G5, R, G5, D6, D6, D6, E6, C6, G5, E6, D6, R, G5, G5, G5, FX6, D6, R, G5, G5, G5, FX6, D6, R, G5, G5, G5, FX6, E6, FX6, G6, G5, G5, G5, G5, R};
static uint8_t notes_duration_SUPERMAN[SONG_LEN_SUPERMAN] = {
        16, 16, 16, 16, 8, 16, 64, 16, 16, 16, 16, 16,128,16, 16, 16, 16, 16,8, 16, 64, 16, 16, 16, 16, 16, 16, 64, 16, 16, 16, 16,  64, 32,16, 16, 16, 16,  64, 32,16, 16, 16, 16,  16, 16,  16, 64, 16, 16, 16, 64, 64};

static uint8_t notes_pitch_EUROPE[SONG_LEN_EUROPE] = {
        CX6, B5, R, CX6, R, FX5, R, D6, R, CX6, R, D6, R, CX6, R, B5, R, D6, R, CX6, R, D6, FX5, R, B5, R, A5, R, B5, R, A5, R, GX5, R, B5, R, A5, CX6, R, B5, R, CX6, FX5, R, R, D6, R, CX6, R, D6, R, CX6, R, B5, R, D6, R, CX6, R, D6, FX5, R, B5, R, A5, R, B5, R, A5, R, GX5, R, B5, R, A5, R, GX5, R, A5, R, B5, A5, B5, CX6, B5, A5, GX5, FX5, D6, CX6, R, CX6, D6, CX6, B5, CX6, R};
static uint8_t notes_duration_EUROPE[SONG_LEN_EUROPE] = {
        8, 4, 4, 16, 8, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 32, 32, 4, 4, 4, 4, 32, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 4, 8, 4, 8, 32, 4, 4, 4, 4, 32, 64, 32, 8, 4, 4, 4, 4, 4, 8, 4, 8, 32, 32, 4, 4, 4, 4, 32, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 4, 8, 4, 8, 64, 8, 4, 4, 4, 4, 32, 8, 8, 16, 16, 16, 16, 32, 32, 128, 16, 8, 8, 8, 8, 64, 8};

static uint8_t notes_pitch_INDIANAJONES[SONG_LEN_INDIANAJONES] = {
        E5, R, F5, G5, R, C6, R, D5, R, E5, F5, R, G5, R, A5, B5, R, F6, R, A5, R, B5, C6, D6, E6, E5, R, F5, G5, R, C6, R, D6, R, E6, F6, G5, R, G5, E6, R, D6, R, G5, E6, R, D6, R, G5, F6, R, E6, R, D6, C6};
static uint8_t notes_duration_INDIANAJONES[SONG_LEN_INDIANAJONES] = {
        32, 16, 16, 16, 16, 64, 16, 32, 16, 16, 128, 32, 32, 16, 16, 16, 16, 64, 32, 32, 16, 16, 64, 64, 64, 32, 16, 16, 16, 16, 128, 32, 32, 16, 16, 128, 32, 16, 16, 32, 16, 32, 16, 16, 32, 16, 32, 16, 16, 32, 16, 32, 16, 16, 128};


static uint8_t notes_pitch_ONE_BEAT[SONG_LEN_ONE_BEAT] = {
  A6, R
};

static uint8_t notes_duration_ONE_BEAT[SONG_LEN_ONE_BEAT] = {
    30, 255};

static uint8_t notes_pitch_TWO_BEAT[SONG_LEN_TWO_BEAT] = {
  A6, R, A6, R
};

static uint8_t notes_duration_TWO_BEAT[SONG_LEN_TWO_BEAT] = {
    30, 15, 30, 255};

static uint8_t notes_pitch_THREE_BEAT[SONG_LEN_THREE_BEAT] = {
  A6, R, A6, R, A6, R};

static uint8_t notes_duration_THREE_BEAT[SONG_LEN_THREE_BEAT] = {
  30, 15, 30, 15, 30, 255};


static uint8_t notes_pitch_SOPRANO_BEAT[SOPRANO_LEN_BEAT] = {
  A6, R
};

static uint8_t notes_duration_SOPRANO_BEAT[SOPRANO_LEN_BEAT] = {
  128, 10
};


static uint8_t notes_pitch_TENOR_BEAT[TENOR_LEN_BEAT] = {
  G5, R,
};

static uint8_t notes_duration_TENOR_BEAT[TENOR_LEN_BEAT] = {
  128, 10
};


static uint8_t notes_pitch_BASS_BEAT[BASS_LEN_BEAT] = {
  E4, R,
};

static uint8_t notes_duration_BASS_BEAT[BASS_LEN_BEAT] = {
  128, 10
};

  

// Returns the number of songs in the jukebox
uint8_t init_jukebox_songs() {

  jukebox_song_list[SONG_SUPERMAN_ORDER].length = SONG_LEN_SUPERMAN;
  jukebox_song_list[SONG_SUPERMAN_ORDER].bpm = SONG_VEL_SUPERMAN;
  jukebox_song_list[SONG_SUPERMAN_ORDER].notes_pitch = notes_pitch_SUPERMAN;
  jukebox_song_list[SONG_SUPERMAN_ORDER].notes_duration = notes_duration_SUPERMAN;

  jukebox_song_list[SONG_INDIANAJONES_ORDER].length = SONG_LEN_INDIANAJONES;
  jukebox_song_list[SONG_INDIANAJONES_ORDER].bpm = SONG_VEL_INDIANAJONES;
  jukebox_song_list[SONG_INDIANAJONES_ORDER].notes_pitch = notes_pitch_INDIANAJONES;
  jukebox_song_list[SONG_INDIANAJONES_ORDER].notes_duration = notes_duration_INDIANAJONES;

  jukebox_song_list[SONG_EUROPE_ORDER].length = SONG_LEN_EUROPE;
  jukebox_song_list[SONG_EUROPE_ORDER].bpm = SONG_VEL_EUROPE;
  jukebox_song_list[SONG_EUROPE_ORDER].notes_pitch = notes_pitch_EUROPE;
  jukebox_song_list[SONG_EUROPE_ORDER].notes_duration = notes_duration_EUROPE;


  /* Beat songs */
  jukebox_song_list[SONG_ONE_BEAT_ORDER].length = SONG_LEN_ONE_BEAT;
  jukebox_song_list[SONG_ONE_BEAT_ORDER].bpm = SONG_VEL_ONE_BEAT;
  jukebox_song_list[SONG_ONE_BEAT_ORDER].notes_pitch = notes_pitch_ONE_BEAT;
  jukebox_song_list[SONG_ONE_BEAT_ORDER].notes_duration = notes_duration_ONE_BEAT;

  jukebox_song_list[SONG_TWO_BEAT_ORDER].length = SONG_LEN_TWO_BEAT;
  jukebox_song_list[SONG_TWO_BEAT_ORDER].bpm = SONG_VEL_TWO_BEAT;
  jukebox_song_list[SONG_TWO_BEAT_ORDER].notes_pitch = notes_pitch_TWO_BEAT;
  jukebox_song_list[SONG_TWO_BEAT_ORDER].notes_duration = notes_duration_TWO_BEAT;

  jukebox_song_list[SONG_THREE_BEAT_ORDER].length = SONG_LEN_THREE_BEAT;
  jukebox_song_list[SONG_THREE_BEAT_ORDER].bpm = SONG_VEL_THREE_BEAT;
  jukebox_song_list[SONG_THREE_BEAT_ORDER].notes_pitch = notes_pitch_THREE_BEAT;
  jukebox_song_list[SONG_THREE_BEAT_ORDER].notes_duration = notes_duration_THREE_BEAT;
  
  jukebox_song_list[SOPRANO_BEAT_ORDER].length = SOPRANO_LEN_BEAT;
  jukebox_song_list[SOPRANO_BEAT_ORDER].bpm = SOPRANO_VEL_BEAT;
  jukebox_song_list[SOPRANO_BEAT_ORDER].notes_pitch = notes_pitch_SOPRANO_BEAT;
  jukebox_song_list[SOPRANO_BEAT_ORDER].notes_duration = notes_duration_SOPRANO_BEAT;

  jukebox_song_list[TENOR_BEAT_ORDER].length = TENOR_LEN_BEAT;
  jukebox_song_list[TENOR_BEAT_ORDER].bpm = TENOR_VEL_BEAT;
  jukebox_song_list[TENOR_BEAT_ORDER].notes_pitch = notes_pitch_TENOR_BEAT;
  jukebox_song_list[TENOR_BEAT_ORDER].notes_duration = notes_duration_TENOR_BEAT;

  jukebox_song_list[BASS_BEAT_ORDER].length = BASS_LEN_BEAT;
  jukebox_song_list[BASS_BEAT_ORDER].bpm = BASS_VEL_BEAT;
  jukebox_song_list[BASS_BEAT_ORDER].notes_pitch = notes_pitch_BASS_BEAT;
  jukebox_song_list[BASS_BEAT_ORDER].notes_duration = notes_duration_BASS_BEAT;
  
  return JUKEBOX_SONGS_TOTAL_SONGS;
}

/* 
 * @brief gets the number of songs
 */
uint8_t get_total_songs_number() {
	return JUKEBOX_SONGS_TOTAL_SONGS;
}


