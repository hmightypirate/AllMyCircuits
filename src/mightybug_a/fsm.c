#include "fsm.h"

static state_e current_state = CALLIBRATION_STATE;
static rnstate_e running_state = RUNNING_NORMAL;
uint32_t running_loop_millisecs = 0;

// Variables for handling target velocity in normal mode
static uint16_t seq_decrease_line_pos = 0;
static uint16_t seq_increase_line_pos = 0;

//Mapping circuit vars
mapping_e mapping_circuit;
uint16_t curr_mapping_pointer = 0;
mapstate_e curr_mapstate = NONE;
uint32_t curr_agg_left_ticks = 0;
uint32_t curr_agg_right_ticks = 0;


/* FIXME this should be moved to a *.h */
/* pid maps: k_p, k_i, k_d */
const int16_t pid_maps[NUMBER_PIDVEL_MAPPINGS * 3] = {
  400, 0, 500,
  400, 0, 500,
  400, 0, 500
};

const int16_t vel_maps[NUMBER_PIDVEL_MAPPINGS] = {
  425, 475, 575
};

const int16_t pid_nool_maps[NUMBER_PIDVEL_MAPPINGS * 3] = {
  400, 0, 600,
  400, 0, 600,
  400, 0, 600
};

const int16_t vel_nool_maps[NUMBER_PIDVEL_MAPPINGS] = {
  350, 350, 350
};


const int16_t pid_turbo_maps[NUMBER_PIDVEL_MAPPINGS * 3] = {
  350, 0, 600,
  350, 0, 600,
  350, 0, 600
};

const int16_t vel_turbo_maps[NUMBER_PIDVEL_MAPPINGS] = {
  450, 500, 525
};

const int16_t normal_out_hyst = OUT_NORMAL_HYST;    // going out of pid (position)
const int16_t turbo_out_hyst = OUT_TURBO_HYST;  // going out of turbo (position)

const int16_t normal_nool_out_hyst = OUT_NORMAL_NOOL_HYST;
const int16_t nool_normal_out_hyst = OUT_NOOL_NORMAL_HYST;


const uint8_t map_songs[MAX_MAPPINGS] = {
  SONG_ONE_BEAT_ORDER, SONG_TWO_BEAT_ORDER, SONG_THREE_BEAT_ORDER
};

uint32_t delay_start_ms = 0;
uint32_t pidvel_map_ms = 0;
uint8_t current_pidvel_mapping = INITIAL_PIDVEL_MAPPING;

uint32_t last_ms_inline = 0;


/* 
 * @brief get mapping state
 */
mapping_e get_mapping_info()
{
  return mapping_circuit;
}

/*
 * @brief update the mapping pointer taking into account the repetitions (if flag set)
 */
void update_map_pointer(void)
{
  curr_mapping_pointer += 1;

  if (FLAG_MAPPING_REPS && mapping_circuit.rep_pointer != -1 && curr_mapping_pointer == mapping_circuit.rep_pointer)
    {
      curr_mapping_pointer = mapping_circuit.large_stline_pointer;
    }
}

/* 
 * @brief check if two ticks corresponds to the same state (aproximate)
 */
uint8_t aprox_stline_equal(uint32_t new_stline_ticks,
			   uint32_t total_stline_ticks)
{
  if (new_stline_ticks > total_stline_ticks)
    {
      if (new_stline_ticks - total_stline_ticks < DIFF_TICKS_EQUAL)
	{
	  return 1;
	}
      else
	{
	  return 0;
	}
    }
  else
    {
      if (total_stline_ticks - new_stline_ticks < DIFF_TICKS_EQUAL)
	{
	  return 1;
	}
      else
	{
	  return 0;
	}
    }
}

/*
 * @brief add ticks to previous state
 *
 *  NOTE: this is only done during the first mapping
 */
void add_map_prevstate(uint32_t curr_agg_left_ticks, uint32_t curr_agg_right_ticks)
{

  if ((curr_mapping_pointer - 1 < MAX_MAP_STATES) &&
      (curr_mapping_pointer - 1 > 0))
    {
      mapping_circuit.agg_left_ticks[curr_mapping_pointer-1] += curr_agg_left_ticks;
      mapping_circuit.agg_right_ticks[curr_mapping_pointer-1] += curr_agg_right_ticks;
    }
}

/*
 * @brief save the state if it is a new state and get a pointer to the next state
 */
void update_map_state(mapstate_e state, uint32_t left_ticks, uint32_t right_ticks)
{

  /* only do something if it has not exceeded the number of mappings */
  if (curr_mapping_pointer < MAX_MAP_STATES)
    {
      if (mapping_circuit.mapstates[curr_mapping_pointer] != NONE)
	{
	  if (state != mapping_circuit.mapstates[curr_mapping_pointer])
	    {
	      // something wrong happened
	      // TODO: flag? rewrite?
	    }
	}
      else
	{
	  // reached a new state

	  // Check if the new state is different than the previous one
	  if ((curr_mapping_pointer > 0) &&
	      (state == mapping_circuit.mapstates[curr_mapping_pointer-1]))
	    {
	      // we are still in the previous state
	      curr_mapping_pointer -= 1;
	    }
	  
	  mapping_circuit.mapstates[curr_mapping_pointer] = state;
	  // adds ticks to the previous state (if it is calculated in several steps)
	  mapping_circuit.agg_left_ticks[curr_mapping_pointer] += curr_agg_left_ticks;
	  mapping_circuit.agg_right_ticks[curr_mapping_pointer] += curr_agg_right_ticks;

	  // search the largest straight line
	  // have seen this stline before?
	  if (state == ST_LINE)
	    {
	      if (mapping_circuit.large_stline_pointer == -1) 
		{
		  mapping_circuit.large_stline_pointer = curr_mapping_pointer;
		}
	      // it is the largest stline
	      else
		{
		  uint32_t total_stline_ticks = mapping_circuit.agg_left_ticks[mapping_circuit.large_stline_pointer] + mapping_circuit.agg_right_ticks[mapping_circuit.large_stline_pointer];

		  uint32_t new_stline_ticks = left_ticks + right_ticks;

		  if (aprox_stline_equal(new_stline_ticks, total_stline_ticks))
		    {
		      // set the pointer to the circuit repetition
		      if (mapping_circuit.rep_pointer != -1)
			{
			  mapping_circuit.rep_pointer = curr_mapping_pointer;
			}
		    }
		  else
		    {
		      // Change the largest line if not equal
		      if (new_stline_ticks > total_stline_ticks)
			{
			  mapping_circuit.large_stline_pointer = curr_mapping_pointer;
			}

		    }
		}
	    }
	}
    }
      
  //Reset aggregation
  curr_agg_left_ticks = 0;
  curr_agg_right_ticks = 0;

  // update pointer
  update_map_pointer();
}

/* 
 * @brief Reset the pointer to the map
 */
void reset_mapping_pointer()
{
  curr_mapping_pointer = 0;
  curr_agg_left_ticks = 0;
  curr_agg_right_ticks = 0;
}
 

/* 
 *  @brief reset mappings
 */
void reset_circuit_mapping()
{

  for (int i=0; i < MAX_MAP_STATES; i++)
    {
      mapping_circuit.agg_left_ticks[i] = 0;
      mapping_circuit.agg_right_ticks[i] = 0;
      mapping_circuit.mapstates[i] = NONE;
    }
  
  curr_mapstate = mapping_circuit.mapstates[0];
  mapping_circuit.large_stline_pointer = -1;
  mapping_circuit.rep_pointer = -1;
  curr_agg_left_ticks = 0;
  curr_agg_right_ticks = 0;

  reset_mapping_pointer();
}

uint8_t reach_consolidated_state(uint32_t agg_left_ticks,
				 uint32_t agg_right_ticks)
{
  if ((agg_left_ticks > MIN_TICKS_FOR_MAP) ||
      (agg_right_ticks > MIN_TICKS_FOR_MAP))
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

/*
 * @brief circuit map
 */
void do_circuit_mapping()
{
  // aggregated ticks
  uint32_t left_ticks = get_left_encoder_ticks();
  uint32_t right_ticks = get_right_encoder_ticks();

  // difference between encoders
  int16_t diff_encoders = get_abs_diff_encoders();
  
  // last ticks
  uint32_t last_left_ticks = get_last_left_ticks();
  uint32_t last_right_ticks = get_last_right_ticks();

  //Init the current mapping (if none)
  if (curr_mapstate == NONE)
    {
      if (diff_encoders < OUT_MAPSTLINE_STATE)
	{
	  curr_mapstate = ST_LINE;
	}
      else
	{
	  if (last_left_ticks > last_right_ticks)
	    {
	      curr_mapstate = RIGHT_CORNER;
	    }
	  else
	    {
	      curr_mapstate = LEFT_CORNER;
	    }
	}
    }

  // Check if we need to store/change mapping
  // Change stline -> corner (any)
  if (curr_mapstate == ST_LINE && diff_encoders > OUT_MAPSTLINE_STATE)
    {
      
      if (reach_consolidated_state(curr_agg_left_ticks, curr_agg_right_ticks))
	{
	  //save state
	  update_map_state(curr_mapstate, curr_agg_left_ticks, curr_agg_right_ticks);
	}
      else
	{
	  // TODO add to prev state?, join states if equal
	  add_map_prevstate(curr_agg_left_ticks, curr_agg_right_ticks);
	  curr_agg_left_ticks = 0;
	  curr_agg_right_ticks = 0;
	}
     
      // a corner, update only the state
      if (left_ticks > right_ticks)
	{
	  curr_mapstate = RIGHT_CORNER;
	}
      else
	{
	  curr_mapstate = LEFT_CORNER;
	}
    }
  
  // change corner (any) -> stline
  else if (curr_mapstate != ST_LINE && diff_encoders <= OUT_MAPCORNER_STATE)
    {
      if (reach_consolidated_state(curr_agg_left_ticks, curr_agg_right_ticks))
	{
	  //save state
	  update_map_state(curr_mapstate, curr_agg_left_ticks, curr_agg_right_ticks);
	}
        else
	  {
	    // TODO add to prev state?, join states if equal
	    add_map_prevstate(curr_agg_left_ticks, curr_agg_right_ticks);
	    curr_agg_left_ticks = 0;
	    curr_agg_right_ticks = 0;
	  }

      curr_mapstate = ST_LINE;
    }

  // change corner (right) -> corner(left)
  else if (curr_mapstate == RIGHT_CORNER && (left_ticks < right_ticks))
    {
	  
      if (reach_consolidated_state(curr_agg_left_ticks, curr_agg_right_ticks))
	{
	  //save state
	  update_map_state(curr_mapstate, curr_agg_left_ticks, curr_agg_right_ticks);
	}
      else
	{
	  // TODO add to prev state?, join states if equal
	  add_map_prevstate(curr_agg_left_ticks, curr_agg_right_ticks);
	  curr_agg_left_ticks = 0;
	  curr_agg_right_ticks = 0;
	}

      // update state
      curr_mapstate = LEFT_CORNER;
    }
  // change corner (left) -> corner (right)
  else if (curr_mapstate == LEFT_CORNER && (right_ticks < left_ticks))
    {
      
      if (reach_consolidated_state(curr_agg_left_ticks, curr_agg_right_ticks))
	{
	  //save state
	  update_map_state(curr_mapstate, curr_agg_left_ticks, curr_agg_right_ticks);
	}
      else
	{
	  // TODO add to prev state?, join states if equal
	  add_map_prevstate(curr_agg_left_ticks, curr_agg_right_ticks);
	  curr_agg_left_ticks = 0;
	  curr_agg_right_ticks = 0;
	}

      // update state
      curr_mapstate = RIGHT_CORNER;
    }
      
  // Aggregate ticks
  curr_agg_left_ticks += last_left_ticks;
  curr_agg_right_ticks += last_right_ticks;
}



/*
 * @brief gets the current ms inline
 */
void update_ms_inline(uint32_t current_ms)
{
  last_ms_inline = current_ms;
}

/*
 * @brief checks if the car has exceeded the time out of line
 */
uint8_t exceeds_time_out_of_line(uint32_t current_ms)
{
  if ((current_ms - last_ms_inline) > MS_DELAY_OUT_OF_LINE)
    {
      return 1;
    }
  else return 0;
}

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
      else if (new_event == GO_TO_NOOL_EVENT && ENABLE_NOOL_MODE)
	{
	  current_state = SET_NOOL_MODE_STATE;
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
 * @brief set state
 */
void set_state(state_e state)
{
  current_state = state;
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
  reset_target_velocity_normal(vel_maps[current_pidvel_mapping]);

  /* reset target turbo velocity */
  reset_target_velocity_turbo(vel_turbo_maps[current_pidvel_mapping]);

  /* reset target turbo velocity */
  reset_target_velocity_nool(vel_nool_maps[current_pidvel_mapping]);  
  
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
          update_state(GO_TO_TURBO_EVENT);
	}
      else if (avg_proportional > normal_nool_out_hyst)
	{
	  update_state(GO_TO_NOOL_EVENT);
	}
    }
  else if (running_state == RUNNING_STLINE)
    {
      if (avg_proportional > turbo_out_hyst)
        {
          update_state(GO_TO_NORMAL_EVENT);
        }
    }
  else if (running_state == RUNNING_NOOL)
    {
      if (avg_proportional < nool_normal_out_hyst)
	{
	  update_state(GO_TO_NORMAL_EVENT);
	}
    }
}


/* 
 * @brief obtains the aggregate number of pos readings improving/decreasing line position
 *
 */
void update_sequential_readings(int16_t new_proportional, int16_t past_proportional)
{
  if (new_proportional < 0)
    {
      new_proportional = -new_proportional;
    }

  if (past_proportional < 0)
    {
      past_proportional = -past_proportional;
    }

  if (new_proportional > past_proportional)
    {
      seq_increase_line_pos += 1;
      seq_decrease_line_pos = 0;
    }
  else
    {
      seq_decrease_line_pos += 1;
      seq_increase_line_pos = 0;
    }

}

void reset_sequential_readings(void)
{
  seq_decrease_line_pos = 0;
  seq_increase_line_pos = 0;
}

/* 
 * @brief updates the target velocity
 *
 */
void update_target_normal()
{
  /* only works in normal mode */
  if (running_state == RUNNING_NORMAL)
    {
      if ((seq_decrease_line_pos > DEC_NORMAL_THRESHOLD) && (get_target_velocity() < MAX_VEL_MOTOR_DEC_MODE))
	{
	  reset_target_velocity(get_target_velocity() + DEC_NORMAL_QTY);
	  if (RESET_DEC_AFTER_SET)
	    {
	      seq_decrease_line_pos = 0;
	    }
	}
      else if ((seq_increase_line_pos > INC_NORMAL_THRESHOLD) && (get_target_velocity() > MIN_VEL_MOTOR_INC_MODE))
	{
	  reset_target_velocity(get_target_velocity() + INC_NORMAL_QTY);
	  if (RESET_INC_AFTER_SET)
	    {
	      seq_increase_line_pos = 0;
	    }
	}
    }
}

/* 
 * @brief updates the target velocity
 *
 */
void update_target_normal_with_encoders()
{

  /* only works in normal mode */
  if (running_state == RUNNING_NORMAL)
    {
      int32_t diff_acc = 0;
      // is left wheel running faster than right wheel 
      if (get_left_encoder_ticks() > get_right_encoder_ticks())
	{
	  // right wheel is acc faster ?
	  diff_acc = get_right_acc() - get_left_acc();
	}
      else
	{
	  diff_acc = get_left_acc() - get_right_acc();
	}

      int32_t next_vel = vel_maps[current_pidvel_mapping] + STEP_NORMAL_QTY * diff_acc;	

      /*
      if (next_vel < MIN_VEL_MOTOR_INC_MODE)
	{
	  next_vel = MIN_VEL_MOTOR_INC_MODE;
	}
      else if (next_vel > MAX_VEL_MOTOR_DEC_MODE)
	{
	  next_vel = MAX_VEL_MOTOR_DEC_MODE;
	}
      */
      
      reset_target_velocity(next_vel);      
    }
  
}

