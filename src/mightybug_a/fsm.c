#include "fsm.h"

static state_e current_state = CALLIBRATION_STATE;


/* FIXME this should be moved to a *.h */
/* pid maps: k_p, k_i, k_d */
const int16_t pid_maps[NUMBER_PID_MAPPINGS * 3] = {
  200, 0, 1800,
  120, 0, 1200,
  100, 0, 0
};

const int16_t vel_maps[NUMBER_VEL_MAPPINGS] = {
  400, 350, 200
};

const uint8_t map_songs[MAX_MAPPINGS] = {
  SONG_ONE_BEAT_ORDER, SONG_TWO_BEAT_ORDER, SONG_THREE_BEAT_ORDER
};

uint32_t delay_start_ms = 0;
uint32_t pid_map_ms = 0;
uint32_t vel_map_ms = 0;

uint8_t current_pid_mapping = INITIAL_PID_MAPPING;
uint8_t current_vel_mapping = INITIAL_VEL_MAPPING;

void reset_mappings(void)
{
  /* change the pid consts */
  set_kp(pid_maps[current_pid_mapping * 3]);
  set_ki(pid_maps[current_pid_mapping * 3 + 1]);
  set_kd(pid_maps[current_pid_mapping * 3 + 2]);
  
  /* change the vel consts */
  reset_target_velocity(vel_maps[current_vel_mapping]);
}

/*
 * @brief extremely simple finite state machine
 */
void update_state(event_e new_event)
{
  if (current_state != NO_BATTERY_STATE)
    {
      if (new_event == FORCE_CALLIBRATION_EVENT)
        {
          current_state = CALLIBRATION_STATE;
        }
      else if (new_event == GO_TO_RUN_EVENT)
        {
          current_state = RUNNING_STATE;
        }
      else if (new_event == OUT_OF_BATTERY_EVENT)
        {
          current_state = NO_BATTERY_STATE;
        }
      else if (new_event == GO_TO_DELAYED_START_EVENT)
        {
          current_state = DELAYED_START_STATE;
        }
      else if ((new_event == NEXT_PIDMAP_EVENT) &&
               (current_state == CALLIBRATION_STATE))       
        {
          /* enable jukebox during mapping selection */
          push_enable_jukebox();
          enable_jukebox();
          current_state = PID_MAPPING_STATE;
        }
      else if ((new_event == NEXT_PIDMAP_EVENT) &&
               (current_state == PID_MAPPING_STATE))
        {
          current_state = PID_CHANGE_STATE;
        }
      else if ((new_event == NEXT_VELMAP_EVENT) &&
               (current_state == CALLIBRATION_STATE))
        {
          /* enable jukebox during mappling selection */
          push_enable_jukebox();
          enable_jukebox();
          current_state = VEL_MAPPING_STATE;
        }
      else if ((new_event == NEXT_VELMAP_EVENT) &&
               (current_state == VEL_MAPPING_STATE))
        {
          current_state = VEL_CHANGE_STATE;
        }
      else if (new_event == FORCE_PIDMAP_EVENT)
        {
          current_state = PID_MAPPING_STATE;
        }
      else if (new_event == FORCE_VELMAP_EVENT)
        {
          current_state = VEL_MAPPING_STATE;
        }
    }
}


/*
 * @brief sets the time entering a pid mapping state
 */ 
void set_pid_map_time(uint32_t current_time)
{
  pid_map_ms = current_time;
}

/*
 * @brief obtain the time entering a pid mapping state
 */
uint32_t get_pid_map_time(void)
{
  return pid_map_ms;
}

/*
 * @brief sets the time entering a vel mapping state
 */
void set_vel_map_time(uint32_t current_time)
{
  vel_map_ms = current_time;
}

/*
 * @brief gets the time entering a vel mapping state
 */
uint32_t get_vel_map_time(void)
{
  return vel_map_ms;
}

/*
 * @brief modify the delay start time
 */
void set_delay_start_time(uint32_t delay)
{
  delay_start_ms = delay;
}

uint32_t get_delay_start_time()
{
  return delay_start_ms;
}

/*
 * @brief get current state
 */
state_e get_state()
{
  return current_state;
}

/* 
 * @brief obtain next pid mapping
 */
void select_next_pid_map()
{
  current_pid_mapping = (current_pid_mapping + 1) % NUMBER_PID_MAPPINGS;

  /* change the pid consts */
  set_kp(pid_maps[current_pid_mapping * 3]);
  set_ki(pid_maps[current_pid_mapping * 3 + 1]);
  set_kd(pid_maps[current_pid_mapping * 3 + 2]);
}

/* 
 * @brief obtain next vel mapping
 */
void select_next_vel_map()
{
  current_vel_mapping = (current_vel_mapping + 1) % NUMBER_VEL_MAPPINGS;

  /* change the vel consts */
  reset_target_velocity(vel_maps[current_vel_mapping]);
}


/*
 * @brief return the current pid mapping
 */
uint8_t get_current_pid_map()
{
  return current_pid_mapping;
}


/*
 * @brief return the current vel mapping
 */
uint8_t get_current_vel_map()
{
  return current_vel_mapping;

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
  
