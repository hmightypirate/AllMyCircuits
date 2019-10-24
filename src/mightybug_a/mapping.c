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
mapstate_e measured_sector_type = NONE;
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
uint8_t end_of_linear_mapping_run = 0;

// Circular mapping structs

// start_loop_sector should point to the id of the next sector after the
// largest straight line in the first pass
int32_t start_loop_sector = 0;
int32_t size_largest_rect = 0;
int16_t largest_st_line_last_sector = -1;
int32_t largest_st_line_size = 0;

// end_loop_sector should point to the id of the next sector after
// the largest straight line in the second pass
int32_t end_loop_sector = 0;

int32_t get_largest_rect_size()
{
	return size_largest_rect;
}

int32_t get_start_loop_sector()
{
	return start_loop_sector;
}

int32_t get_end_loop_sector()
{
	return end_loop_sector;
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

void increase_sector_data(uint16_t index, int32_t left_ticks,
			  int32_t right_ticks, int32_t total_ticks)
{
	mapping_circuit.agg_total_ticks[index] += total_ticks;
	mapping_circuit.agg_left_ticks[index] += left_ticks;
	mapping_circuit.agg_right_ticks[index] += right_ticks;
}

/*
 * @brief adding a sector to the current mapping
 */
void add_sector_to_list(mapstate_e new_state)
{

	if (curr_mapping_pointer >= MAX_MAP_STATES) {
		return;
	}

	// Check if it is in the first sector
	// or different kind of previous stored sector
	if ((curr_mapping_pointer == 0) ||
	    (mapping_circuit.mapstates[curr_mapping_pointer - 1] !=
	     new_state)) {
		// New sector
		set_sector_data(curr_mapping_pointer, new_state,
				first_tick_sector, curr_agg_left_ticks,
				curr_agg_right_ticks, curr_agg_total_ticks);
		curr_mapping_pointer += 1;
	} else {
		// Join to previous sector (these might
		// only happen with UNKNOWN states)
		increase_sector_data(curr_mapping_pointer - 1,
				     curr_agg_left_ticks, curr_agg_right_ticks,
				     curr_agg_total_ticks);
	}

	first_tick_sector += curr_agg_total_ticks;
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
	start_loop_sector = largest_st_line_last_sector + 1;
	end_loop_sector = last_sector + 1;

	// get ticks run after largest st line
	int32_t extra_ticks = 0;
	for (int i = end_loop_sector; i <= curr_mapping_pointer; i++) {
		extra_ticks += mapping_circuit.agg_total_ticks[i];
	}

	// advancing the ticks in the synchro sectors to the appr sector
	// check if we already complete some sectors after st line
	uint16_t approx_sync_sector = start_loop_sector;
	while (extra_ticks >
	       mapping_circuit.agg_total_ticks[approx_sync_sector]) {
		extra_ticks -=
		    mapping_circuit.agg_total_ticks[approx_sync_sector++];
	}

	// preparing the data for synchro
	// Ticks of the next sector already advanced
	meas_l_ticks = extra_ticks;
	meas_r_ticks = extra_ticks;
	meas_agg_ticks = extra_ticks;
	meas_total_ticks =
	    (mapping_circuit.first_tick[approx_sync_sector] + extra_ticks) * 2;

	sync_next_sector_idx = approx_sync_sector;
	get_next_sector();

	// End record mapping, start run synchro
	synchro_mapping_flag = 1;
}

/*
 * @brief obtain straight line
 */
void check_circular_stline(int16_t index)
{

	// last sector should point to the last sector of a straight line
	int16_t new_st_line_last_sector = -1;

	// sector size should contain the length of a straight line
	int32_t new_st_line_size = 0;

	for (; index >= 0; index--) {
		if (mapping_circuit.mapstates[index] == UNKNOWN) {
			if (new_st_line_size > 0) {
				new_st_line_size +=
				    mapping_circuit.agg_total_ticks[index];
			}
		} else if (mapping_circuit.mapstates[index] == ST_LINE) {
			if (new_st_line_last_sector < 0) {
				new_st_line_last_sector = index;
			}
			new_st_line_size +=
			    mapping_circuit.agg_total_ticks[index];
		} else
			break;
	}

	if (new_st_line_last_sector == -1) {
		// no st_line detected
		return;
	}

	if (new_st_line_size < CIRCULAR_TICKS_MINSTLINE) {
		// rect hast to be of a minimum size
		return;
	}

	if (new_st_line_last_sector == largest_st_line_last_sector) {
		// Only continue this process if there is a straight line (and
		// it is not the already detected one)
		return;
	}

	if (largest_st_line_last_sector == -1) {
		// first st_line detected
		largest_st_line_size = new_st_line_size;
		return;
	}

	if (abs(new_st_line_size - largest_st_line_size) <
	    CIRCULAR_TICKS_MINSTLINE) {
		// Probably it is repeating the same st_line
		jump_to_circular_synchro(new_st_line_last_sector);
	} else if (new_st_line_size > largest_st_line_size) {
		// new largest st_line
		largest_st_line_last_sector = new_st_line_last_sector;
		largest_st_line_size = new_st_line_size;
	}
}

void add_sector()
{
	if (curr_agg_total_ticks < MIN_SECTOR_LENGTH) {
		add_sector_to_list(UNKNOWN);
	} else {
		// Adding a sector that has the minimum length
		add_sector_to_list(curr_mapstate);

		// Search for largst rect
		if (DO_CIRCULAR_MAPPING) {
			if ((curr_mapping_pointer > 1) &&
			    (curr_mapstate != ST_LINE))
				check_circular_stline(curr_mapping_pointer - 2);
		}
	}
}

mapstate_e get_sector_type_from_encoder_ticks()
{
	// aggregated ticks
	uint32_t left_ticks = get_left_encoder_ticks();
	uint32_t right_ticks = get_right_encoder_ticks();

	// difference between encoders
	int16_t diff_encoders = get_abs_diff_encoders();

	if (diff_encoders < OUT_MAPSTLINE_STATE) {
		return ST_LINE;
	} else if (left_ticks > right_ticks) {
		return RIGHT_CORNER;
	} else {
		return LEFT_CORNER;
	}

	return UNKNOWN;
}

/*
 * @brief circuit map
 */
void record_mapping()
{
	mapstate_e new_measured_sector_type =
	    get_sector_type_from_encoder_ticks();

	if (curr_mapstate != new_measured_sector_type) {
		add_sector();

		curr_agg_total_ticks = 0;
		curr_agg_left_ticks = 0;
		curr_agg_right_ticks = 0;
	}

	curr_mapstate = new_measured_sector_type;
	curr_agg_left_ticks += get_last_left_ticks();
	curr_agg_right_ticks += get_last_right_ticks();
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
	measured_sector_type = NONE;
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

bool can_join_two_additional_sectors()
{
	return (sync_next_sector_idx + 2 < MAX_MAP_STATES &&
		mapping_circuit.mapstates[sync_next_sector_idx] == UNKNOWN &&
		mapping_circuit.mapstates[sync_next_sector_idx + 1] ==
		    mapping_circuit.mapstates[sync_sector_idx]);
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
	while (can_join_two_additional_sectors()) {
		sync_sector_length +=
		    (mapping_circuit.agg_total_ticks[sync_next_sector_idx] +
		     mapping_circuit.agg_total_ticks[sync_next_sector_idx + 1]);

		sync_sector_end =
		    (mapping_circuit.first_tick[sync_next_sector_idx + 1] +
		     mapping_circuit.agg_total_ticks[sync_next_sector_idx + 1]);

		sync_next_sector_idx += 2;
	}
}

bool can_join_next_sectors()
{
	return (sync_sector_idx + 2 < MAX_MAP_STATES &&
		mapping_circuit.mapstates[sync_sector_idx] != UNKNOWN &&
		mapping_circuit.mapstates[sync_sector_idx + 1] == UNKNOWN &&
		mapping_circuit.mapstates[sync_sector_idx + 2] ==
		    mapping_circuit.mapstates[sync_sector_idx]);
}

/*
 * @brief obtain next sector
 */
void get_next_sector()
{

	sync_sector_idx = sync_next_sector_idx;

	if (DO_CIRCULAR_MAPPING) {
		// Check if we have finished the lap
		if (sync_sector_idx >= end_loop_sector) {
			sync_sector_idx = start_loop_sector;

			// updating the total ticks (as it has
			// overflowed)
			meas_total_ticks =
			    (mapping_circuit.first_tick[sync_sector_idx] * 2);
		}
	}

	if (sync_sector_idx >= MAX_MAP_STATES - 1) {
		// Reached maximum number of states
		sync_next_sector_idx = sync_sector_idx;
		load_synchro_sector(MAX_MAP_STATES - 1);
		end_of_linear_mapping_run = 1;
		return;
	}

	if (mapping_circuit.mapstates[sync_sector_idx + 1] == NONE) {
		// Reached the last mapped state
		sync_next_sector_idx = sync_sector_idx;
		load_synchro_sector(sync_sector_idx);
		end_of_linear_mapping_run = 1;
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
void round_synchro()
{
	sync_change_flag = 0;

	if (sync_sector_type == UNKNOWN)
		return;

	int32_t diff_ticks = abs(meas_total_ticks / 2 - sync_sector_end);

	if (diff_ticks < SYNCHRO_MAX_DRIFT) {
		meas_total_ticks = sync_sector_end * 2;
		get_next_sector();
	} else {
		diff_ticks = abs(meas_total_ticks / 2 -
				 mapping_circuit.first_tick[sync_sector_idx]);

		if (diff_ticks < SYNCHRO_MAX_DRIFT) {
			meas_total_ticks =
			    mapping_circuit.first_tick[sync_sector_idx] * 2;
		}
	}
}

/*
  @brief obtain current state estimation using stored mapping information

 */
void synchro_mapping(void)
{
	mapstate_e new_measured_sector_type =
	    get_sector_type_from_encoder_ticks();

	// last ticks (last ms)
	uint32_t last_left_ticks = get_last_left_ticks();
	uint32_t last_right_ticks = get_last_right_ticks();

	// First sector
	if (sync_sector_type == NONE) {
		get_next_sector(); // Get next sector
	}

	// if changed measured sector type (line sensors)
	if ((measured_sector_type != NONE) &&
	    (measured_sector_type != new_measured_sector_type)) {

		if (meas_agg_ticks > MIN_SECTOR_LENGTH) {
			// Get synchro
			round_synchro();
		}

		meas_l_ticks = 0;
		meas_r_ticks = 0;
		meas_agg_ticks = 0;
	}

	measured_sector_type = new_measured_sector_type;

	meas_l_ticks += last_left_ticks;
	meas_r_ticks += last_right_ticks;
	meas_agg_ticks = (meas_l_ticks + meas_r_ticks) / 2;
	meas_total_ticks += (last_left_ticks + last_right_ticks);

	// if detected sector is not the synchro (mapping)
	if (sync_sector_type != measured_sector_type) {
		if (meas_total_ticks / 2 > sync_sector_end) {
			meas_total_ticks = sync_sector_end * 2;

			get_next_sector();

			if (measured_sector_type == sync_sector_type) {
				meas_total_ticks += 2 * meas_agg_ticks;
			}
		}
	}
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
uint8_t get_synchro_flag(void)
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

uint8_t get_end_of_linear_mapping_run(void)
{
	return end_of_linear_mapping_run;
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
