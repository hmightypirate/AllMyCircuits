#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#include "jukebox_songs.h"

/* Number of mappings for pid and vel */
#define MAX_MAPPINGS 3
#define INITIAL_PIDVEL_MAPPING 0

/* FIXME this should be moved to a *.h */
/* pid maps: k_p, k_i, k_d */
static const int16_t pid_maps[MAX_MAPPINGS * 3] = {200, 0, 300, // 400, 0, 500
						   200, 0, 300, 200, 0, 300};

static const int16_t vel_maps[MAX_MAPPINGS] = {
    // 650, 675, 725
    // 600, 650, 700 // mapping 1st test
    // 700, 750, 780 // mapping 2nd test
    650, 725, 800 // mapping vel normal
};

// Best mapping 600/525; 650/525/35 (1 vuelta -morro corto), 650/515/25 (morro
// corto stripped)

static const int16_t pid_nool_maps[MAX_MAPPINGS * 3] = {400, 0,   600, 400, 0,
							600, 400, 0,   600};

static const int16_t vel_nool_maps[MAX_MAPPINGS] = {
    350, 350, 350 // mapping vel nool
};

static const int16_t pid_turbo_maps[MAX_MAPPINGS * 3] = {
    200, 0, 300, // 350, 0, 600
    200, 0, 300, 200, 0, 300};

static const int16_t vel_turbo_maps[MAX_MAPPINGS] = {
    // 515, 515, 515
    // 575, 600, 625 //mapping 1st test
    // 625, 650, 675 //mapping 2nd test
    600, 650, 700 // mapping vel turbo
};

static const uint8_t map_songs[MAX_MAPPINGS] = {
    SONG_ONE_BEAT_ORDER, SONG_TWO_BEAT_ORDER, SONG_THREE_BEAT_ORDER};

#endif