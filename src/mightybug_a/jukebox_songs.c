#include "jukebox_songs.h"


static uint8_t notes_pitch_LIGHTMYFIRE[SONG_LEN_LIGHTMYFIRE] = {
        B5, G5, A5, B5, D6, C6, B5, A5, G5, A5, F5, A5, C6, F6, D6, C6, AX5, G5, GX5, G5, GX5, G5, A5, B5, CX6, B5, A5, G5, F5, E5, F5, A5, A5};
static uint8_t notes_duration_LIGHTMYFIRE[SONG_LEN_LIGHTMYFIRE] = {
        16, 8, 8, 16, 16, 16, 16, 16, 16, 16, 8, 8, 16, 16, 8, 8, 8, 8, 16, 16, 16, 8, 8, 16, 16, 8, 8, 8, 8, 16, 16, 128, 32};

static uint8_t notes_pitch_TEARS_IN_HEAVEN[SONG_LEN_TEARS_IN_HEAVEN] = {
        R, B5, G5, D6, D6, B5, A5, G5, R, R, C6, C6, B5, A5, G5, B5, A5};
static uint8_t notes_duration_TEARS_IN_HEAVEN[SONG_LEN_TEARS_IN_HEAVEN] = {
        32, 16, 16, 32, 16, 16, 8, 32, 64, 32, 16, 16, 16, 16, 16, 32, 64};


static uint8_t notes_pitch_EUROPE[SONG_LEN_EUROPE] = {
        CX6, B5, R, CX6, R, FX5, R, D6, R, CX6, R, D6, R, CX6, R, B5, R, D6, R, CX6, R, D6, FX5, R, B5, R, A5, R, B5, R, A5, R, GX5, R, B5, R, A5, CX6, R, B5, R, CX6, FX5, R, R, D6, R, CX6, R, D6, R, CX6, R, B5, R, D6, R, CX6, R, D6, FX5, R, B5, R, A5, R, B5, R, A5, R, GX5, R, B5, R, A5, R, GX5, R, A5, R, B5, A5, B5, CX6, B5, A5, GX5, FX5, D6, CX6, R, CX6, D6, CX6, B5, CX6, R};
static uint8_t notes_duration_EUROPE[SONG_LEN_EUROPE] = {
        8, 4, 4, 16, 8, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 32, 32, 4, 4, 4, 4, 32, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 4, 8, 4, 8, 32, 4, 4, 4, 4, 32, 64, 32, 8, 4, 4, 4, 4, 4, 8, 4, 8, 32, 32, 4, 4, 4, 4, 32, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 4, 8, 4, 8, 64, 8, 4, 4, 4, 4, 32, 8, 8, 16, 16, 16, 16, 32, 32, 128, 16, 8, 8, 8, 8, 64, 8};


// Returns the number of songs in the jukebox
uint8_t init_jukebox_songs() {

    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].length = SONG_LEN_LIGHTMYFIRE;
    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].bpm = SONG_VEL_LIGHTMYFIRE;
    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].notes_pitch = notes_pitch_LIGHTMYFIRE;
    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].notes_duration = notes_duration_LIGHTMYFIRE;

    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].length = SONG_LEN_TEARS_IN_HEAVEN;
    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].bpm = SONG_VEL_TEARS_IN_HEAVEN;
    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].notes_pitch = notes_pitch_TEARS_IN_HEAVEN;
    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].notes_duration = notes_duration_TEARS_IN_HEAVEN;

    jukebox_song_list[SONG_EUROPE_ORDER].length = SONG_LEN_EUROPE;
    jukebox_song_list[SONG_EUROPE_ORDER].bpm = SONG_VEL_EUROPE;
    jukebox_song_list[SONG_EUROPE_ORDER].notes_pitch = notes_pitch_EUROPE;
    jukebox_song_list[SONG_EUROPE_ORDER].notes_duration = notes_duration_EUROPE;

    return JUKEBOX_SONGS_TOTAL_SONGS;
}

/* 
 * @brief gets the number of songs
 */
uint8_t get_total_songs_number() {
	return JUKEBOX_SONGS_TOTAL_SONGS;
}
