#include "fsm.h"

static state_e current_state = CALLIBRATION_STATE;


/* FIXME this should be moved to a *.h */
/* pid maps: k_p, k_i, k_d */
const int16_t pid_maps[NUMBER_PIDVEL_MAPPINGS * 3] = {
  200, 0, 1800,
  200, 0, 1800,
  200, 0, 1800
};

const int16_t vel_maps[NUMBER_PIDVEL_MAPPINGS] = {
  300, 500, 300
};

const uint8_t map_songs[MAX_MAPPINGS] = {
  SONG_ONE_BEAT_ORDER, SONG_TWO_BEAT_ORDER, SONG_THREE_BEAT_ORDER
};

uint32_t delay_start_ms = 0;
uint32_t pidvel_map_ms = 0;
uint8_t current_pidvel_mapping = INITIAL_PIDVEL_MAPPING;

void reset_mappings(void)
{
  /* change the pid consts */
  set_kp(pid_maps[current_pidvel_mapping * 3]);
  set_ki(pid_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_maps[current_pidvel_mapping * 3 + 2]);
  
  /* change the vel consts */
  reset_target_velocity(vel_maps[current_pidvel_mapping]);
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
          if (get_calibrated_sensors_count() >= NUM_SENSORS -
              MAX_NUM_NOT_CALLIBRATED_SENSORS)
            {
              current_state = RUNNING_STATE;
            }
        }
      else if (new_event == OUT_OF_BATTERY_EVENT)
        {
          current_state = NO_BATTERY_STATE;
        }
      else if (new_event == GO_TO_DELAYED_START_EVENT)
        {
          if (get_calibrated_sensors_count() >= NUM_SENSORS -
              MAX_NUM_NOT_CALLIBRATED_SENSORS)
            {
              current_state = DELAYED_START_STATE;
            }
        }
      else if ((new_event == NEXT_PIDANDVELMAP_EVENT) &&
               (current_state == CALLIBRATION_STATE))
        {
          push_enable_jukebox();
          enable_jukebox();
          current_state = PIDANDVEL_MAPPING_STATE;
        }
      else if ((new_event == NEXT_PIDANDVELMAP_EVENT) &&
               (current_state == PIDANDVEL_MAPPING_STATE))
        {
          current_state = PIDANDVEL_CHANGE_STATE;
        }      
      else if (new_event == FORCE_PIDANDVELMAP_EVENT)
        {
          current_state = PIDANDVEL_MAPPING_STATE;
        }
      else if ((new_event == NEXT_BUZZER_EVENT) &&
               (current_state == CALLIBRATION_STATE))
        {
          if (is_jukebox_enabled())
            {
              disable_jukebox();
            }
          else
            {
              enable_jukebox();
            }
        }                                
    }
}


/*
 * @brief sets the time entering a pid/vel mapping state
 */ 
void set_pidvel_map_time(uint32_t current_time)
{
  pidvel_map_ms = current_time;
}

/*
 * @brief obtain the time entering a pid/vel mapping state
 */
uint32_t get_pidvel_map_time(void)
{
  return pidvel_map_ms;
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
 * @brief obtain next pid/vel mapping
 */
void select_next_pidvel_map()
{
  current_pidvel_mapping = (current_pidvel_mapping + 1) % NUMBER_PIDVEL_MAPPINGS;

  /* change the pid consts */
  set_kp(pid_maps[current_pidvel_mapping * 3]);
  set_ki(pid_maps[current_pidvel_mapping * 3 + 1]);
  set_kd(pid_maps[current_pidvel_mapping * 3 + 2]);

  /* change vel cts */
  reset_target_velocity(vel_maps[current_pidvel_mapping]);
}


/*
 * @brief return the current pid mapping
 */
uint8_t get_current_pidvel_map()
{
  return current_pidvel_mapping;
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
  
