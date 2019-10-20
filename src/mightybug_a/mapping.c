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
uint8_t synchro_mapping_flag = 0;

// Flag to indicate that the end of mapping has been reached
uint8_t end_of_mapping = 0;

// Circular mapping structs

// end_sector_largest_rect should point to the id of the next sector after the
// largest straight line in the first pass
int32_t end_sector_largest_rect = 0;
int32_t size_largest_rect = 0;

// finish_mapping_largest_rect should point to the id of the next sector after
// the largest straight line in the second pass
int32_t finish_mapping_largest_rect = 0;

int32_t get_largest_rect_size()
{
	return size_largest_rect;
}

int32_t get_end_sector_largest_rect()
{
	return end_sector_largest_rect;
}

int32_t get_finish_mapping_largest_rect()
{
	return finish_mapping_largest_rect;
}

void set_sector_data(uint16_t index, mapstate_e type, int32_t first_tick,
		     int32_t left_ticks, int32_t right_ticks,
		     int32_t total_ticks)
{
	mapping_circuit.agg_total_ticks[index] = total_ticks;
	mapping_circuit.agg_left_ticks[index] = left_ticks;
	mapping_circuit.agg_right_ticks[index] = right_ticks;
	mapping_circuit.first_tick[index] = first_tick;
	mapping_circuit.mapstates[index] = type;
}

void add_sector_data(uint16_t index, int32_t left_ticks, int32_t right_ticks,
		     int32_t total_ticks)
{
	mapping_circuit.agg_total_ticks[index] += total_ticks;
	mapping_circuit.agg_left_ticks[index] += left_ticks;
	mapping_circuit.agg_right_ticks[index] += right_ticks;
}

/*
 * @brief adding a sector to the current mapping
 */
void add_map_to_list(mapstate_e new_state)
{

	if (curr_mapping_pointer >= MAX_MAP_STATES) {
		return;
	}

	// Check if it is in the first sector
	// or same kind of previous stored sector
	if ((curr_mapping_pointer == 0) ||
	    (mapping_circuit.mapstates[curr_mapping_pointer - 1] ==
	     new_state)) {
		// New sector
		set_sector_data(curr_mapping_pointer, new_state,
				first_tick_sector, curr_agg_left_ticks,
				curr_agg_right_ticks, curr_agg_total_ticks);
		curr_mapping_pointer += 1;
	} else {
		// Join to previous sector (these might
		// only happen with UNKNOWN states)
		add_sector_data(curr_mapping_pointer - 1, curr_agg_left_ticks,
				curr_agg_right_ticks, curr_agg_total_ticks);
	}

	first_tick_sector += curr_agg_total_ticks;
	curr_agg_total_ticks = 0;
	curr_agg_left_ticks = 0;
	curr_agg_right_ticks = 0;
}

/*
 * @brief get mapping state
 */
mapping_e get_mapping_info()
{
	return mapping_circuit;
}

/*
 * @brief prepare the jump to the synchro mode
 */
void jump_to_circular_synchro(int32_t last_sector)
{

	finish_mapping_largest_rect = last_sector + 1;

	// get ticks run after largest st line
	int32_t extra_ticks = 0;
	for (int i = last_sector + 1; i <= curr_mapping_pointer; i++) {
		extra_ticks += mapping_circuit.agg_total_ticks[i];
	}

	// advancing the ticks in the synchro sectors to the appr sector
	// check if we already complete some sectors after st line
	uint16_t approx_sync_sector = end_sector_largest_rect;
	for (int i = end_sector_largest_rect; i < finish_mapping_largest_rect;
	     i++) {
		if (extra_ticks > mapping_circuit.agg_total_ticks[i]) {
			extra_ticks -= mapping_circuit.agg_total_ticks[i];
			// only advance a sector if the car has traversed the
			// whole sector distance
			approx_sync_sector = i + 1;

		} else {
			break;
		}
	}

	// preparing the data for synchro
	synchro_mapping_flag = 1;
	sync_sector_idx = approx_sync_sector;
	sync_next_sector_idx = sync_sector_idx;

	// Ticks of the next sector already advanced
	meas_l_ticks = extra_ticks;
	meas_r_ticks = extra_ticks;
	meas_agg_ticks = extra_ticks;

	// The car is going to advance to the next sector (picking preemptively
	// the first tick of the next sector)
	meas_total_ticks =
	    (mapping_circuit.first_tick[sync_sector_idx + 1] + extra_ticks) * 2;

	get_next_sector();
}

/*
 * @brief obtain straight line
 */
void check_circular_stline(uint16_t search_pointer)
{

	// last sector should point to the last sector of a straight line
	int32_t last_sector = -1;

	// sector size should contain the length of a straight line
	int32_t sector_size = 0;

	for (int i = search_pointer; i >= 0; i--) {
		if (mapping_circuit.mapstates[i] == UNKNOWN) {
			if (sector_size > 0) {
				sector_size +=
				    mapping_circuit.agg_total_ticks[i];
			}
		} else if (mapping_circuit.mapstates[i] == ST_LINE) {
			if (last_sector < 0) {
				last_sector = i;
			}
			sector_size += mapping_circuit.agg_total_ticks[i];
		} else
			break;
	}

	// Only continue this process if there is a straight line (and it is not
	// the already detected one)
	if ((last_sector >= 0) &&
	    ((last_sector + 1) != end_sector_largest_rect)) {
		// rect hast to be of a minimum size
		if (sector_size > CIRCULAR_TICKS_MINSTLINE) {

			if (size_largest_rect == 0) {
				// first rect detected
				size_largest_rect = sector_size;
				end_sector_largest_rect = last_sector + 1;
			} else if (sector_size > size_largest_rect) {
				if ((sector_size - size_largest_rect) <
				    CIRCULAR_TICKS_STLINE_DIFF) {
					// Probably repeating the largest rect
					// (taking the straight line)
					jump_to_circular_synchro(last_sector);
				} else {
					// A new largest rect
					size_largest_rect = sector_size;
					end_sector_largest_rect =
					    last_sector + 1;
				}
			} else {
				// New rect is shorter than the stored rect
				// checking if it has reached the same rect
				if ((size_largest_rect - sector_size) <
				    CIRCULAR_TICKS_STLINE_DIFF) {
					// Probably repeating the rect (taking
					// the straight line)
					jump_to_circular_synchro(last_sector);
				}
			}
		}
	}
}

/*
 * @brief circuit map
 */
void record_mapping()
{

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
		if (curr_mapstate != NONE && curr_mapstate != ST_LINE) {
			// Adding a new state if its size is greater than the
			// minimum required
			if (curr_agg_total_ticks > MIN_SECTOR_LENGTH) {
				// Adding current sector
				add_map_to_list(curr_mapstate);

				// Search for largst rect
				if (DO_CIRCULAR_MAPPING) {
					if ((curr_mapping_pointer > 1) &&
					    (curr_mapstate != ST_LINE))
						check_circular_stline(
						    curr_mapping_pointer - 2);
				}

			} else {
				// Adding an unknown sector
				// FIXME adding a new sector here
				add_map_to_list(UNKNOWN);
			}
		}
		curr_mapstate = ST_LINE;
	}
	// Check if it is a right corner (left ticks greater than right ticks)
	else if (left_ticks > right_ticks) {
		// Check if we have reached a new state
		if (curr_mapstate != NONE && curr_mapstate != RIGHT_CORNER) {

			if (curr_agg_total_ticks > MIN_SECTOR_LENGTH) {
				// Adding a sector that has the minimum length
				add_map_to_list(curr_mapstate);

				// Search for largst rect
				if (DO_CIRCULAR_MAPPING) {
					if ((curr_mapping_pointer > 1) &&
					    (curr_mapstate != ST_LINE))
						check_circular_stline(
						    curr_mapping_pointer - 2);
				}

			} else {
				// Adding an unknown sector
				add_map_to_list(UNKNOWN);
			}
		}
		curr_mapstate = RIGHT_CORNER;
		// It is a Left corner (right ticks greater than left ticks)
	} else {
		// Check if we have reached a new state
		if (curr_mapstate != NONE && curr_mapstate != LEFT_CORNER) {
			if (curr_agg_total_ticks > MIN_SECTOR_LENGTH) {
				// Adding a right corner sector
				add_map_to_list(curr_mapstate);

				// Search for largest rect
				if (DO_CIRCULAR_MAPPING) {
					if (curr_mapping_pointer > 1)
						if ((curr_mapping_pointer >
						     1) &&
						    (curr_mapstate != ST_LINE))
							check_circular_stline(
							    curr_mapping_pointer -
							    2);
				}

			} else {
				// Adding an unknown sector
				add_map_to_list(UNKNOWN);
			}
		}
		curr_mapstate = LEFT_CORNER;
	}

	curr_agg_left_ticks += last_left_ticks;
	curr_agg_right_ticks += last_right_ticks;

	// Always update the total ticks as the mean of left and right ticks
	// FIXME: this is not the correct way of doing this (better take into
	// account angular, linear vels)
	curr_agg_total_ticks = (curr_agg_left_ticks + curr_agg_right_ticks) / 2;
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

void load_synchro_sector(uint16_t index)
{
	sync_sector_type = mapping_circuit.mapstates[index];
	sync_sector_length = mapping_circuit.agg_total_ticks[index];
	sync_sector_end =
	    mapping_circuit.first_tick[index] + sync_sector_length;
}

void join_and_load_next_sectors()
{
	//  Three sectors can be joined in one if the the sectors in the
	//  ends are the same and the intermediate is unknown.
	sync_next_sector_idx += 3;
	sync_sector_type = mapping_circuit.mapstates[sync_sector_idx];
	sync_sector_length =
	    (mapping_circuit.agg_total_ticks[sync_sector_idx] +
	     mapping_circuit.agg_total_ticks[sync_sector_idx + 1] +
	     mapping_circuit.agg_total_ticks[sync_sector_idx + 2]);
	sync_sector_end =
	    (mapping_circuit.first_tick[sync_sector_idx + 2] +
	     mapping_circuit.agg_total_ticks[sync_sector_idx + 2]);

	// Check if there are more sectors that can be joined in one
	while (1) {
		if (sync_next_sector_idx + 2 < MAX_MAP_STATES &&
		    mapping_circuit.mapstates[sync_next_sector_idx] ==
			UNKNOWN &&
		    mapping_circuit.mapstates[sync_next_sector_idx + 1] ==
			mapping_circuit.mapstates[sync_sector_idx]) {

			sync_sector_length +=
			    (mapping_circuit
				 .agg_total_ticks[sync_next_sector_idx] +
			     mapping_circuit
				 .agg_total_ticks[sync_next_sector_idx + 1]);

			sync_sector_end =
			    (mapping_circuit
				 .first_tick[sync_next_sector_idx + 1] +
			     mapping_circuit
				 .agg_total_ticks[sync_next_sector_idx + 1]);

			sync_next_sector_idx += 2;

		} else {
			break;
		}
	}
}

bool can_join_next_sectors()
{
	return (mapping_circuit.mapstates[sync_sector_idx] != UNKNOWN &&
	    sync_sector_idx + 2 < MAX_MAP_STATES &&
	    mapping_circuit.mapstates[sync_sector_idx + 1] == UNKNOWN &&
	    mapping_circuit.mapstates[sync_sector_idx + 2] ==
		mapping_circuit.mapstates[sync_sector_idx]);
}

/*
 * @brief obtain next sector
 */
void get_next_sector()
{

	if (DO_CIRCULAR_MAPPING) {
		// Check if we have finished the lap
		if (sync_sector_idx >= finish_mapping_largest_rect) {
			sync_sector_idx = end_sector_largest_rect;
			sync_next_sector_idx = sync_sector_idx;

			// updating the total ticks (as it has
			// overflowed)
			meas_total_ticks =
			    (mapping_circuit.first_tick[sync_sector_idx] * 2);
		}
	}

	if (sync_sector_idx >= MAX_MAP_STATES) {
		// Reached maximum number of states (do not adding a new
		// state)
		sync_next_sector_idx = sync_sector_idx;
		load_synchro_sector(MAX_MAP_STATES - 1);
		end_of_mapping = 1;
		return;
	}

	if (mapping_circuit.mapstates[sync_sector_idx + 1] == NONE) {
		// Reached the last mapped state
		sync_next_sector_idx = sync_sector_idx;
		load_synchro_sector(sync_sector_idx);
		end_of_mapping = 1;
		return;
	}

	if (can_join_next_sectors()) {
		join_and_load_next_sectors();
	} else {
		sync_next_sector_idx = sync_sector_idx + 1;
		load_synchro_sector(sync_sector_idx);
	}
}

/*
 * @brief tries to synchronizes the ticks
 */
void round_synchro(mapstate_e map_state)
{
	sync_change_flag = 0;

	int32_t diff_ticks = 0;

	if (map_state != sync_sector_type && sync_sector_type != UNKNOWN) {
		// Try to synchronize
		// Check if it has started a new state

		diff_ticks = abs(meas_total_ticks / 2 -
				 mapping_circuit.first_tick[sync_sector_idx]);

		if (diff_ticks < SYNCHRO_MAX_DRIFT) {
			meas_total_ticks =
			    mapping_circuit.first_tick[sync_sector_idx] * 2;

		}
		// check if it has finished the current state
		else {
			diff_ticks =
			    abs(meas_total_ticks / 2 - sync_sector_end);

			if (diff_ticks < SYNCHRO_MAX_DRIFT) {
				meas_total_ticks = sync_sector_end * 2;
				sync_change_flag = 1;
			}
		}
	} else if (map_state == sync_sector_type) {
		diff_ticks = abs(meas_total_ticks / 2 - sync_sector_end);

		if (diff_ticks < SYNCHRO_MAX_DRIFT) {
			meas_total_ticks = sync_sector_end * 2;
			sync_change_flag = 1;
		}
	}
}

void check_sector_synchronization(mapstate_e state)
{
	if (meas_sector_type != NONE && meas_sector_type != state) {
		if (meas_agg_ticks > MIN_SECTOR_LENGTH) {
			// Get synchro
			round_synchro(meas_sector_type);

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
}

void check_sector_synchronization_change()
{
	if (meas_total_ticks / 2 > sync_sector_end &&
	    sync_sector_type != meas_sector_type) {
		meas_total_ticks = sync_sector_end * 2;

		// Get next state but do not syncronize
		sync_sector_idx = sync_next_sector_idx;

		get_next_sector();

		if (meas_sector_type == sync_sector_type) {
			meas_total_ticks += 2 * meas_agg_ticks;
		}
	}
}

/*
  @brief obtain current state estimation using stored mapping information

 */
void synchro_mapping(void)
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
	if (sync_sector_type == NONE) {
		get_next_sector(); // Get next sector
	}

	// It has reached a posible straight line state
	if (diff_encoders < OUT_MAPSTLINE_STATE) {
		check_sector_synchronization(ST_LINE);
		meas_sector_type = ST_LINE;
	} else if (left_ticks > right_ticks) {
		check_sector_synchronization(RIGHT_CORNER);
		meas_sector_type = RIGHT_CORNER;
	} else {
		check_sector_synchronization(LEFT_CORNER);
		meas_sector_type = LEFT_CORNER;
	}

	meas_l_ticks += last_left_ticks;
	meas_r_ticks += last_right_ticks;
	meas_agg_ticks = (meas_l_ticks + meas_r_ticks) / 2;
	// should divide by 2 but numbers could be very small
	meas_total_ticks += (last_left_ticks + last_right_ticks);

	check_sector_synchronization_change();
}

/*
 * @brief check if we already performed the mapping and we must start the
 * synchronization
 */
void update_synchro_mapping_flag(void)
{
	if (curr_mapping_pointer > 0) {
		synchro_mapping_flag = 1;
	}
}

/*
 * @brief select the function mapping/synchro
 */
void update_mapping(void)
{
	if (synchro_mapping_flag) {
		synchro_mapping();
	} else {
		record_mapping();
	}
}

/*
 * @brief get synchronization flag
 */
uint8_t round_synchro_flag(void)
{
	return synchro_mapping_flag;
}

/*
 * @brief indicate if we can safely increase the velocity
 */
uint8_t is_hyper_turbo_safe(mapstate_e state)
{

	if (synchro_mapping_flag) {
		// Check if it is in the required state
		// if (sync_sector_type == state)
		if (sync_sector_type == state) {
			// Check it is safe to update the velocity
			if (meas_total_ticks / 2 + TURBO_SYNCHRO_TICKS <
			    sync_sector_end) {
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

uint16_t round_synchro_sector_idx(void)
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

mapstate_e round_synchro_state(void)
{
	return sync_sector_type;
}
