#include "mapping.h"


// Mapping circuit vars
mapping_e mapping_circuit;
uint16_t curr_mapping_pointer = 0;
mapstate_e curr_mapstate = NONE;
uint32_t curr_agg_left_ticks = 0;
uint32_t curr_agg_right_ticks = 0;
uint32_t curr_agg_total_ticks = 0;
uint32_t first_tick_sector = 0;

// Synchro mapping vars
mapstate_e meas_sector_type = NONE;
uint32_t meas_l_ticks = 0;
uint32_t meas_r_ticks = 0;
uint32_t meas_agg_ticks = 0;
int32_t meas_total_ticks = 0;
mapstate_e sync_sector_type = NONE;
uint32_t sync_sector_length = -1;
uint16_t sync_sector_idx = 0;
uint16_t sync_sector_end = 0;
uint16_t sync_next_sector_idx = 0;
uint8_t sync_change_flag = 0;

// Flag to switch between mapping and synchro
uint8_t switch_synchro_flag = 0;

// Flag to indicate that the end of mapping has been reached
uint8_t end_of_mapping = 0;

// Circular mapping structs
int32_t end_sector_largest_rect = 0;
int32_t size_largest_rect = 0;
int32_t finish_mapping_largest_rect = 0;

/*

number of ticks ? / number of seconds (estimated given the velocity?)

*/


// number of mapping
// 2 states: doing mapping and running
// doing mapping
// running

// mapping by time/encoders, taking into account current velocity (running safely)
// during mapping, point to a former state ?

// car goes slow/fast ? number of ticks should not differ but it might


/*
 * @brief adding a sector to the current mapping
 */
void adding_map_to_list(mapstate_e new_state)
{
  if (curr_mapping_pointer < MAX_MAP_STATES) {
      // Check if it is in the first sector 
      if (curr_mapping_pointer > 0) {
          if (mapping_circuit.mapstates[curr_mapping_pointer - 1] == new_state) {
	    // Joining sectors (these might only happen with UNKNOWN states)
 	      mapping_circuit.agg_total_ticks[curr_mapping_pointer-1] += curr_agg_total_ticks;
	      mapping_circuit.agg_left_ticks[curr_mapping_pointer-1] += curr_agg_left_ticks;
	      mapping_circuit.agg_right_ticks[curr_mapping_pointer-1] += curr_agg_right_ticks;

	  } else {
       	      // Not aggregating sectors
	      mapping_circuit.agg_total_ticks[curr_mapping_pointer] = curr_agg_total_ticks;
	      mapping_circuit.agg_left_ticks[curr_mapping_pointer] = curr_agg_left_ticks;
	      mapping_circuit.agg_right_ticks[curr_mapping_pointer] = curr_agg_right_ticks;
	      mapping_circuit.first_tick[curr_mapping_pointer] = first_tick_sector;
	      mapping_circuit.mapstates[curr_mapping_pointer] = new_state;
	      curr_mapping_pointer += 1;
	   }
      } else {
	  // First sector
	  mapping_circuit.agg_total_ticks[curr_mapping_pointer] = curr_agg_total_ticks;
	  mapping_circuit.agg_left_ticks[curr_mapping_pointer] = curr_agg_left_ticks;
	  mapping_circuit.agg_right_ticks[curr_mapping_pointer] = curr_agg_right_ticks;
	  mapping_circuit.first_tick[curr_mapping_pointer] = first_tick_sector;
	  mapping_circuit.mapstates[curr_mapping_pointer] = new_state;

	  curr_mapping_pointer += 1;
	}
    }

    first_tick_sector += curr_agg_total_ticks;
    curr_agg_total_ticks = 0;
    curr_agg_left_ticks = 0;
    curr_agg_right_ticks = 0;
}


/*
 * @brief get mapping state
 */
mapping_e get_mapping_info() {
    return mapping_circuit;
}


/*
 * @brief obtain straight line
 */
void check_circular_stline(uint16_t search_pointer) {

  int32_t last_sector = -1;
  int32_t sector_size = 0;

  for (int i = search_pointer; i >= 0; i--) {
    if (mapping_circuit.mapstates[i] == UNKNOWN) {
      if (sector_size > 0)
	{
	  sector_size += mapping_circuit.agg_total_ticks[i];
	}
    }
    else if (mapping_circuit.mapstates[i] == ST_LINE) {
      if (last_sector < 0)
	{
	  last_sector = search_pointer;
	}
      sector_size += mapping_circuit.agg_total_ticks[i];
    }
    else
      break;
  }

  if (last_sector > 0) {
      if (sector_size > CIRCULAR_TICKS_MINSTLINE) {
	  if (sector_size > size_largest_rect) {
	    if ((sector_size - size_largest_rect) < CIRCULAR_TICKS_STLINE_DIFF) {
	      // Probably repeating the rect (taking the straight line)
	      finish_mapping_largest_rect = last_sector + 1;

	      // TODO: go to synchro mode
	      
	    }
	  }
	  else {
	    // A new largest rect
	    size_largest_rect = sector_size;
	    end_sector_largest_rect = last_sector;
	  }
      }

      else {
	// The first rect
 	size_largest_rect = sector_size;
	end_sector_largest_rect = last_sector;	
      }
  }
}


/*
 * @brief circuit map
 */
void do_circuit_mapping() {

    // aggregated ticks
    uint32_t left_ticks = get_left_encoder_ticks();
    uint32_t right_ticks = get_right_encoder_ticks();

    // difference between encoders
    int16_t diff_encoders = get_abs_diff_encoders();

    // last ticks measured by the encoders
    uint32_t last_left_ticks = get_last_left_ticks();
    uint32_t last_right_ticks = get_last_right_ticks();

    // It has reached a possible straight line state
    if (diff_encoders < OUT_MAPSTLINE_STATE) {
        if (curr_mapstate != NONE &&
	    curr_mapstate != ST_LINE) {
	    // Adding a new state if its size is greater than the minimum required
	    if (curr_agg_total_ticks > MIN_SECTOR_LENGTH) {
	        // Adding a stline sector
                adding_map_to_list(curr_mapstate);
	     } else {
	         // Adding an unknown sector
		 //FIXME adding a new sector here
		 adding_map_to_list(UNKNOWN);
	     }
	  }
	  curr_mapstate = ST_LINE;
     }
     // Check if it is a Right corner (left ticks greater than right ticks)
     else if (left_ticks > right_ticks) {
         // Check if we have reached a new state
	 if (curr_mapstate != NONE &&
	    curr_mapstate != RIGHT_CORNER) {

	     if (curr_agg_total_ticks > MIN_SECTOR_LENGTH) {
	         // Adding a right corner sector
		 adding_map_to_list(curr_mapstate);

		 // TODO: Check for straight line

		 if (DO_CIRCULAR_MAPPING) {
		   if (curr_mapping_pointer > 1)
		     check_circular_stline(curr_mapping_pointer - 2);
		   }
		 
	     } else {
	         // Adding an unknown sector
	         adding_map_to_list(UNKNOWN);
	     }
	  }
	  curr_mapstate = RIGHT_CORNER;
      // It is a Left corner (right ticks greater than left ticks)
      } else {
	  // Check if we have reached a new state
	  if (curr_mapstate != NONE &&
	    curr_mapstate != LEFT_CORNER) {
	      if (curr_agg_total_ticks > MIN_SECTOR_LENGTH) {
	          // Adding a right corner sector
	          adding_map_to_list(curr_mapstate);

		  // TODO: Check for straight line
		  if (DO_CIRCULAR_MAPPING) {
		    
		    if (curr_mapping_pointer > 1)
		      check_circular_stline(curr_mapping_pointer - 2);
		  }
		  
	       } else {
	           // Adding an unknown sector
	           adding_map_to_list(UNKNOWN);
	       }
	   }
	   curr_mapstate = LEFT_CORNER;
      }
	  
     curr_agg_left_ticks += last_left_ticks;
     curr_agg_right_ticks += last_right_ticks;

     // Always update the total ticks as the mean of left and right ticks
     // FIXME: this is not the correct way of doing this (better take into account angular, linear vels)
     curr_agg_total_ticks = (curr_agg_left_ticks + curr_agg_right_ticks)/2;
}


/*
 * @brief Reset the pointer to the map
 */
void reset_mapping_pointer()
{
	curr_mapping_pointer = 0;
	curr_agg_total_ticks = 0;
	curr_agg_left_ticks = 0;
	curr_agg_right_ticks = 0;
	first_tick_sector = 0;
	curr_mapstate = NONE;
}

/*
 *  @brief reset mappings
 */
void reset_circuit_mapping()
{
    for (int i = 0; i < MAX_MAP_STATES; i++) {
        mapping_circuit.agg_left_ticks[i] = 0;
	mapping_circuit.agg_right_ticks[i] = 0;
	mapping_circuit.agg_total_ticks[i] = 0;
	mapping_circuit.mapstates[i] = NONE;
	mapping_circuit.first_tick[i] = 0;
     }

    reset_mapping_pointer();
}

/*
 * @brief reset synchro information
 */
void reset_synchro(void)
{
    meas_agg_ticks = 0;
    meas_total_ticks = 0;
    meas_l_ticks = 0;
    meas_r_ticks = 0;
    meas_sector_type = NONE;
    sync_sector_type = NONE;
    sync_sector_length = -1;
    sync_sector_idx = 0;
    sync_sector_end = 0;
    sync_next_sector_idx = 0;
    sync_change_flag = 0;
}
 

/*
 * @brief obtain next sector
 */
void get_next_sector() {

    if (sync_sector_idx >= MAX_MAP_STATES) {
        // Reached maximum number of states (do not adding a new state)
	sync_next_sector_idx = sync_sector_idx;
	sync_sector_type = mapping_circuit.mapstates[MAX_MAP_STATES -1];
	sync_sector_length = mapping_circuit.agg_total_ticks[MAX_MAP_STATES - 1];
	sync_sector_end = mapping_circuit.first_tick[MAX_MAP_STATES-1] + sync_sector_length;

	end_of_mapping = 1;
	return;
    }
		
    if (mapping_circuit.mapstates[sync_sector_idx + 1] == NONE) {
        // Reached the last mapped state
	sync_next_sector_idx = sync_sector_idx;
	sync_sector_type = mapping_circuit.mapstates[sync_sector_idx];
	sync_sector_length = mapping_circuit.agg_total_ticks[sync_sector_idx];
	sync_sector_end = mapping_circuit.first_tick[sync_sector_idx] + sync_sector_length;
	end_of_mapping = 1;
	
	return;
    }

    if (mapping_circuit.mapstates[sync_sector_idx] != UNKNOWN &&
        MAX_MAP_STATES < sync_sector_idx + 2 && 
	mapping_circuit.mapstates[sync_sector_idx + 1] == UNKNOWN &&
	mapping_circuit.mapstates[sync_sector_idx + 2] == mapping_circuit.mapstates[sync_sector_idx]) {

        //  Three sectors can be joined in one if the the sectors in the ends are the same and the intermediate is unknown.
        sync_next_sector_idx += 3;
	sync_sector_type = mapping_circuit.mapstates[sync_sector_idx];
	sync_sector_length = (mapping_circuit.agg_total_ticks[sync_sector_idx] +
			      mapping_circuit.agg_total_ticks[sync_sector_idx + 1] +
			      mapping_circuit.agg_total_ticks[sync_sector_idx + 2]);
	sync_sector_end = (mapping_circuit.first_tick[sync_sector_idx + 2] +
		  mapping_circuit.agg_total_ticks[sync_sector_idx + 2]);

	// Check if there are more sectors that can be joined in one
	while(1) {
	    if (sync_next_sector_idx + 2 < MAX_MAP_STATES &&
	          mapping_circuit.mapstates[sync_next_sector_idx] == UNKNOWN &&
		  mapping_circuit.mapstates[sync_next_sector_idx + 1] ==
		  mapping_circuit.mapstates[sync_sector_idx]) {

	         sync_sector_length += (mapping_circuit.agg_total_ticks[sync_next_sector_idx] + mapping_circuit.agg_total_ticks[sync_next_sector_idx + 1]);

		 sync_sector_end = (mapping_circuit.first_tick[sync_next_sector_idx + 1] + mapping_circuit.agg_total_ticks[sync_next_sector_idx + 1]);

		sync_next_sector_idx += 2;
	
	     } else {
	         break;
	     }
	 }		  
    } else {
      sync_next_sector_idx = sync_sector_idx + 1;
      sync_sector_type = mapping_circuit.mapstates[sync_sector_idx];
      sync_sector_length = mapping_circuit.agg_total_ticks[sync_sector_idx];
      sync_sector_end = mapping_circuit.first_tick[sync_sector_idx] + sync_sector_length;
      
    }
}

/*
 * @brief tries to synchronizes the ticks 
 */
void get_synchro(mapstate_e map_state)
{
  sync_change_flag = 0;

  int32_t diff_ticks = 0;
  
  if (map_state != sync_sector_type &&
      sync_sector_type != UNKNOWN) {
    // Try to synchronize
    // Check if it has started a new state

    diff_ticks = meas_total_ticks/2 - mapping_circuit.first_tick[sync_sector_idx];
    
    if (diff_ticks < 0) {
      diff_ticks *= -1;
    }

    if (diff_ticks < SYNCHRO_MAX_DRIFT) {
      meas_total_ticks = mapping_circuit.first_tick[sync_sector_idx] * 2;
      
    }
    // check if it has finished the current state
    else {
	diff_ticks = meas_total_ticks/2 - sync_sector_end;
	if (diff_ticks < 0) {
	  diff_ticks *= -1;
	}

	if (diff_ticks < SYNCHRO_MAX_DRIFT)
	  {
	    meas_total_ticks = sync_sector_end * 2;
	    sync_change_flag = 1;
	  }
      }
    } else if (map_state == sync_sector_type) {
        diff_ticks = meas_total_ticks/2 - sync_sector_end;
        if (diff_ticks < 0) {
           diff_ticks *= -1;
        }

        if (diff_ticks < SYNCHRO_MAX_DRIFT)
       {
          meas_total_ticks = sync_sector_end * 2;
          sync_change_flag = 1;
       }
    }
}

 
/*
  @brief obtain current state estimation using stored mapping information

 */ 
void do_synchro_run(void)
{
	// aggregated ticks of the last x ms
	uint32_t left_ticks = get_left_encoder_ticks();
	uint32_t right_ticks = get_right_encoder_ticks();

	// difference between encoders
	int16_t diff_encoders = get_abs_diff_encoders();

	// last ticks (last ms)
	uint32_t last_left_ticks = get_last_left_ticks();
	uint32_t last_right_ticks = get_last_right_ticks();

	// First sector
	if (sync_sector_type == NONE)
	  {
	    get_next_sector(); // Get next sector
	  }

	// It has reached a posible straight line state
	if (diff_encoders < OUT_MAPSTLINE_STATE) {
	     if (meas_sector_type != NONE &&
		  meas_sector_type != ST_LINE) {
	         if (meas_agg_ticks > MIN_SECTOR_LENGTH) {
		     // Get synchro
		     get_synchro(meas_sector_type);

		     if (sync_change_flag) {
		        sync_sector_idx = sync_next_sector_idx;

			// Get next sector
			get_next_sector();
			
         	     }
	          }
		 meas_l_ticks = 0;
		 meas_r_ticks = 0;
		 meas_agg_ticks = 0; 
	      }

	     meas_sector_type = ST_LINE;
	 }

	else if (left_ticks > right_ticks)
	  {
	     if (meas_sector_type != NONE &&
		 meas_sector_type != RIGHT_CORNER) {
	         if (meas_agg_ticks > MIN_SECTOR_LENGTH) {
		     // Get synchro
		     get_synchro(meas_sector_type);

		     if (sync_change_flag) {
		        sync_sector_idx = sync_next_sector_idx;

			// Get next sector
			get_next_sector();
         	     }		
	          }
		 meas_l_ticks = 0;
		 meas_r_ticks = 0;
		 meas_agg_ticks = 0; 
	      }

	     meas_sector_type = RIGHT_CORNER;
	  }
	else
	  {
	     if (meas_sector_type != NONE &&
		  meas_sector_type != LEFT_CORNER) {
	         if (meas_agg_ticks > MIN_SECTOR_LENGTH) {
		     // Get synchro
		     get_synchro(meas_sector_type);

		     if (sync_change_flag) {
		        sync_sector_idx = sync_next_sector_idx;

			// Get next sector
			get_next_sector();
         	     }		
	          }
		 meas_l_ticks = 0;
		 meas_r_ticks = 0;
		 meas_agg_ticks = 0; 
	      }

	     meas_sector_type = LEFT_CORNER;
	   }

	meas_l_ticks += last_left_ticks;
	meas_r_ticks += last_right_ticks;
	meas_agg_ticks = (meas_l_ticks + meas_r_ticks)/2;
	// should divide by 2 but numbers could be very small
	meas_total_ticks += (last_left_ticks + last_right_ticks);

	if (meas_total_ticks/2 > sync_sector_end &&
	    sync_sector_type != meas_sector_type)
	  {
	    meas_total_ticks = sync_sector_end * 2;

	    // Get next state but do not syncronize
	    sync_sector_idx = sync_next_sector_idx;

	    get_next_sector();

	    if (meas_sector_type == sync_sector_type)
	      {
	          meas_total_ticks += 2 * meas_agg_ticks;
	      }
	  }	
}


/*
 * @brief check if we already performed the mapping and we must start the synchronization
 */
void check_synchro_start(void)
{
  if (curr_mapping_pointer > 0)
    {
      switch_synchro_flag = 1;
    }
}


/*
 * @brief select the function mapping/synchro
 */
void update_mapping_function(void)
{
  if (switch_synchro_flag)
    {
        do_synchro_run();
    }
  else
    {
        do_circuit_mapping();
    }
}

/*
 * @brief get synchronization flag
 */
uint8_t get_mapping_function(void)
{
  return switch_synchro_flag;
}


/* 
 * @brief indicate if we can safely increase the velocity
 */
uint8_t is_increase_vel_enable(mapstate_e state)
{
  if (switch_synchro_flag)
    {
      // Check if it is in the required state
      //if (sync_sector_type == state)
      if (sync_sector_type == state)
	 {
	  // Check it is safe to update the velocity
	   if (meas_total_ticks/2 + TURBO_SYNCHRO_TICKS < sync_sector_end)
	   {
	      return 1;
	   }
	}
    }

  return 0;
}

uint8_t get_end_of_mapping(void)
{
  return end_of_mapping;
}

uint16_t get_synchro_sector_idx(void)
{
  return sync_sector_idx;
}

uint16_t get_mapping_pointer_idx(void)
{
  return curr_mapping_pointer;
}

mapstate_e get_mapping_state(void)
{
  return curr_mapstate;
}

mapstate_e get_synchro_state(void)
{
  return sync_sector_type;
}
