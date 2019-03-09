#ifndef __PIDVELMAP_H
#define __PIDVELMAP_H

#include "configs.h"
#include "motors.h"
#include "pid.h"
#include <stdint.h>

void reset_pids_normal(void);
void reset_pids_turbo(void);
void reset_pids_nool();
void select_next_pidvel_map(void);
uint8_t get_current_pidvel_map(void);
uint8_t get_map_song(uint8_t id_map);

/* Target velocity modes */
void set_target_as_turbo(void);
void set_target_as_brake(void);
void set_target_as_normal(void);
void set_target_as_nool(void);

#endif // __PIDVELMAP_H