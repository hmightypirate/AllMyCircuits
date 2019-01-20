#include "mapping.h"

//Mapping circuit vars
mapping_e mapping_circuit;
uint16_t curr_mapping_pointer = 0;
mapstate_e curr_mapstate = NONE;
uint32_t curr_agg_left_ticks = 0;
uint32_t curr_agg_right_ticks = 0;

//vel delay features
veldelay_e veldelay_st;




/*
 * @brief helper function to truncate a value between min and max
 */
static int32_t trunc_to_range(int32_t value, int32_t min, int32_t max)
{
  int trunc_value = value;

  if (value < min)
    trunc_value = min;
  else if (value > max)
    trunc_value = max;

  return trunc_value;
}

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
    return (new_stline_ticks - total_stline_ticks) < DIFF_TICKS_EQUAL;
  }
  else
  {
    return (total_stline_ticks - new_stline_ticks) < DIFF_TICKS_EQUAL;
  }
}

/*
 * @brief add ticks to previous state
 *
 *  NOTE: this is only done during the first mapping
 */
void add_map_prevstate()
{

  if ((curr_mapping_pointer - 1 < MAX_MAP_STATES) &&
      (curr_mapping_pointer - 1 > 0) &&
      (mapping_circuit.rep_pointer == -1)) //only update if we are not repeating again the circuit
  {
    mapping_circuit.agg_left_ticks[curr_mapping_pointer - 1] += curr_agg_left_ticks;
    mapping_circuit.agg_right_ticks[curr_mapping_pointer - 1] += curr_agg_right_ticks;
  }
}

/*
 * @brief save the state if it is a new state and get a pointer to the next state
 */
void update_map_state(mapstate_e state)
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
          (state == mapping_circuit.mapstates[curr_mapping_pointer - 1]))
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

          // the new line is the current
          uint32_t new_stline_ticks = mapping_circuit.agg_left_ticks[curr_mapping_pointer] + mapping_circuit.agg_right_ticks[curr_mapping_pointer];

          if (aprox_stline_equal(new_stline_ticks, total_stline_ticks))
          {
            // set the pointer to the circuit repetition (do only once)
            if (mapping_circuit.rep_pointer == -1)
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

  for (int i = 0; i < MAX_MAP_STATES; i++)
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
      update_map_state(curr_mapstate);
    }
    else
    {
      // TODO add to prev state?, join states if equal
      add_map_prevstate();
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
      update_map_state(curr_mapstate);
    }
    else
    {
      // TODO add to prev state?, join states if equal
      add_map_prevstate();
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
      update_map_state(curr_mapstate);
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
      update_map_state(curr_mapstate);
    }
    else
    {
      // TODO add to prev state?, join states if equal
      add_map_prevstate();
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
 * @brief reset the mapping 
 */
void reset_veldelay(void)
{
  for (int i = 0; i < MAX_VEL_DELAY; i++)
  {
    veldelay_st.motor_vel[i] = 0;
  }

  veldelay_st.current_pointer = 0;
  veldelay_st.total_samples = 0;
}

/*
 * @brief obtain the next constrained target velocity
 */
int32_t get_next_constrained_target_velocity(int32_t vel)
{
  uint16_t next_pointer = veldelay_st.current_pointer + 1;
  next_pointer = veldelay_st.current_pointer %= MAX_VEL_DELAY;

  return trunc_to_range(vel,
                        veldelay_st.motor_vel[next_pointer] -
                            MAX_VEL_DELAY_STEP_DOWN,
                        veldelay_st.motor_vel[next_pointer] +
                            MAX_VEL_DELAY_STEP_UP);
}

/*
 * @brief increse the pointer in the vel delay struct
 */
void increase_pointer_vel_delay(int32_t last_vel)
{
  veldelay_st.current_pointer += 1;
  veldelay_st.current_pointer %= MAX_VEL_DELAY;

  veldelay_st.motor_vel[veldelay_st.current_pointer] = last_vel;

  veldelay_st.total_samples += 1;
}

