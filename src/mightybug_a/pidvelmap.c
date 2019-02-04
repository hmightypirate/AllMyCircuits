#include "pidvelmap.h"

uint8_t current_pidvel_mapping = INITIAL_PIDVEL_MAPPING;

/* 
 * @brief obtain next pid/vel mapping
 */
void select_next_pidvel_map(void)
{
  current_pidvel_mapping = (current_pidvel_mapping + 1) % MAX_MAPPINGS;
}

/* 
 * @brief change the pid consts to the normal mapping
*/
void reset_pids_normal()
{
  /* change the pid consts */
  set_kp(pid_maps[current_pidvel_mapping * 3]);
  set_ki(pid_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_maps[current_pidvel_mapping * 3 + 2]);
}

/* 
 * @brief change the pid consts to the turbo mapping
*/
void reset_pids_turbo()
{
  /* change the pid consts */
  set_kp(pid_turbo_maps[current_pidvel_mapping * 3]);
  set_ki(pid_turbo_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_turbo_maps[current_pidvel_mapping * 3 + 2]);
}

/* 
 * @brief change the pid consts to the nool mapping
*/
void reset_pids_nool()
{
  /* change the pid consts */
  set_kp(pid_nool_maps[current_pidvel_mapping * 3]);
  set_ki(pid_nool_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_nool_maps[current_pidvel_mapping * 3 + 2]);
}

/*
 * @brief return the current pid mapping
 */
uint8_t get_current_pidvel_map()
{
  return current_pidvel_mapping;
}