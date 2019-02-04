#ifndef __PIDVELMAP_H
#define __PIDVELMAP_H

#include <stdint.h>
#include "configs.h"
#include "pid.h"

void reset_pids_normal(void);
void reset_pids_turbo(void);
void reset_pids_nool();
void select_next_pidvel_map(void);
uint8_t get_current_pidvel_map(void);

#endif // __PIDVELMAP_H