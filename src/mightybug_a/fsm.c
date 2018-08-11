#include "fsm.h"

static state_e current_state = CALLIBRATION_STATE;
static rnstate_e running_state = RUNNING_NORMAL;
uint32_t running_loop_millisecs = 0;

/* FIXME this should be moved to a *.h */
/* pid maps: k_p, k_i, k_d */
const int16_t pid_maps[NUMBER_PIDVEL_MAPPINGS * 3] = {
  400, 0, 1600,
  400, 0, 1600,
  400, 0, 1600
};

const int16_t vel_maps[NUMBER_PIDVEL_MAPPINGS] = {
  300, 450, 600
};

const int16_t pid_turbo_maps[NUMBER_PIDVEL_MAPPINGS * 3] = {
  300, 0, 1600,
  300, 0, 1600,
  300, 0, 1600
};

const int16_t vel_turbo_maps[NUMBER_PIDVEL_MAPPINGS] = {
  500, 500, 500
};

const int16_t normal_out_hyst = OUT_NORMAL_HYST;    // going out of pid (position)
const int16_t turbo_out_hyst = OUT_TURBO_HYST;  // going out of turbo (position)

const uint8_t map_songs[MAX_MAPPINGS] = {
  SONG_ONE_BEAT_ORDER, SONG_TWO_BEAT_ORDER, SONG_THREE_BEAT_ORDER
};

uint32_t delay_start_ms = 0;
uint32_t pidvel_map_ms = 0;
uint8_t current_pidvel_mapping = INITIAL_PIDVEL_MAPPING;


/*
 * @brief get the ms the car entered the running state
 */
uint32_t get_running_ms()
{
  return running_loop_millisecs;
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
              // Set the ms at the start of the running state
              running_loop_millisecs = get_millisecs_since_start();
            }          
        }
      else if (new_event == OUT_OF_BATTERY_EVENT)
        {
          current_state = NO_BATTERY_STATE;
        }
      else if (new_event == GO_TO_DELAYED_START_EVENT)
        {
          /* if in callibration -> go to delay start  */
          if (current_state == CALLIBRATION_STATE)
            {
              if (get_calibrated_sensors_count() >= NUM_SENSORS -
                  MAX_NUM_NOT_CALLIBRATED_SENSORS)
                {
                  current_state = DELAYED_START_STATE;
                }
            }
          /* if running -> go to idle state */
          else if (current_state == RUNNING_STATE)
            {
              current_state = IDLE_STATE;
              
            }
          /* if in any other state -> go to callibration state */
          else
            {
              current_state = CALLIBRATION_STATE;
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
      else if (new_event == FORCE_IDLE_EVENT)
        {
          current_state = IDLE_STATE;
        }
      else if (new_event == GO_TO_TURBO_EVENT && ENABLE_TURBO_MODE)
        {
          // resets the time in corner          
          current_state = SET_TURBO_MODE_STATE;
        }
      else if (new_event == GO_TO_NORMAL_EVENT && ENABLE_TURBO_MODE)
        {
          current_state = SET_NORMAL_MODE_STATE;
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
 * @brief set running state
 */
rnstate_e get_running_state()
{
  return running_state;
}

void set_running_state(rnstate_e state)
{
  running_state = state;
}

/* 
 * @brief obtain next pid/vel mapping
 */
void select_next_pidvel_map()
{
  current_pidvel_mapping = (current_pidvel_mapping + 1) % NUMBER_PIDVEL_MAPPINGS;

  force_mapping_to_current();
}

void force_mapping_to_current()
{
  /* reset pids normal */
  reset_pids_normal();

  /* change vel cts */
  reset_target_velocity(vel_maps[current_pidvel_mapping]);

  /* reset target turbo velocity */
  reset_target_velocity_turbo(vel_turbo_maps[current_pidvel_mapping]);  
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
  
/*
 * @brief get next sub-state (running)
 *
 */
void get_next_running_state(int16_t avg_proportional)
{

  if (running_state == RUNNING_NORMAL)
    {
      if (avg_proportional < normal_out_hyst)
        {
          update_state(GO_TO_TURBO_EVENT);                      }
    }
  else if (running_state == RUNNING_STLINE)
    {
      if (avg_proportional > normal_out_hyst)
        {
          update_state(GO_TO_NORMAL_EVENT);
        }
    }
}
