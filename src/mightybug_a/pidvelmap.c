#include "pidvelmap.h"

uint8_t current_pidvel_mapping = INITIAL_PIDVEL_MAPPING;

/*
 * @brief obtain next pid/vel mapping
 */
void select_next_pidvel_map(void)
{
	current_pidvel_mapping = (current_pidvel_mapping + 1) % NUMBER_OF_MAPPINGS;
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

/*
 * @brief resets current target velocity with the turbo velocity
 *
 */
void set_target_as_turbo(void)
{
	set_target_velocity(vel_turbo_maps[get_current_pidvel_map()]);
}

/*
 * @brief resets current target velocity with the brake velocity
 *
 */
void set_target_as_brake(void)
{
	set_target_velocity(vel_brake_maps[get_current_pidvel_map()]);
}

/*
 * @brief resets current target velocity with the velocity in corners
 *
 */
void set_target_as_nool(void)
{
	set_target_velocity(vel_nool_maps[get_current_pidvel_map()]);
}

/*
 * @brief resets current target velocity with the normal velocity
 *
 */
void set_target_as_normal(void)
{
	set_target_velocity(vel_maps[get_current_pidvel_map()]);
}

/*
 * @brief get current map song
 *
 * @note at least must be id map songs + 1
 */
uint8_t get_map_song(uint8_t id_map)
{
	return map_songs[id_map];
}