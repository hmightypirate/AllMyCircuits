#ifndef __CONFIGS_H
#define __CONFIGS_H

#include "jukebox_songs.h"

/* Number of mappings for pid and vel */
#define NUMBER_OF_MAPPINGS 3
#define INITIAL_PIDVEL_MAPPING 0

/* pid maps: k_p, k_i, k_d */
extern int16_t pid_maps[NUMBER_OF_MAPPINGS * 3];
extern int16_t vel_maps[NUMBER_OF_MAPPINGS];
extern int16_t pid_nool_maps[NUMBER_OF_MAPPINGS * 3];

extern int16_t vel_nool_maps[NUMBER_OF_MAPPINGS];

extern int16_t pid_turbo_maps[NUMBER_OF_MAPPINGS * 3];
extern int16_t vel_turbo_maps[NUMBER_OF_MAPPINGS];
extern int16_t vel_hyper_turbo_maps[NUMBER_OF_MAPPINGS];
 
extern int16_t vel_brake_maps[NUMBER_OF_MAPPINGS];

extern int16_t vel_recovery_maps[NUMBER_OF_MAPPINGS];

extern const uint8_t map_songs[NUMBER_OF_MAPPINGS];

void config_vars(void);

#endif /* __CONFIGS_H */
