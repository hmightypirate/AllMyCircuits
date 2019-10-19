#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "encoder.h"
#include <stdint.h>

/* Mapping variables */
#define FLAG_CIRCUIT_MAPPING 1
#define MAX_MAP_STATES 200
#define OUT_MAPSTLINE_STATE 6 // diff in abs encoder ticks
#define MIN_SECTOR_LENGTH 180 // minimum length of sector
#define SYNCHRO_MAX_DRIFT 200
#define TURBO_SYNCHRO_TICKS 400 // remaining ticks

#define DO_CIRCULAR_MAPPING 1
#define ALLOW_MAPPING_IN_CORNERS 1
#define CIRCULAR_TICKS_STLINE_DIFF                                             \
	150 // 150 ticks of difference of the largest rect
#define CIRCULAR_TICKS_MINSTLINE 2000 // 500 ticks of mininmum straight line

typedef enum {
	NONE,
	UNKNOWN,
	LEFT_CORNER,
	RIGHT_CORNER,
	ST_LINE,
	MAX_MAP_EVENTS
} mapstate_e;

typedef struct {
	int32_t agg_total_ticks[MAX_MAP_STATES];
	int32_t agg_left_ticks[MAX_MAP_STATES];
	int32_t agg_right_ticks[MAX_MAP_STATES];
	int32_t first_tick[MAX_MAP_STATES];
	mapstate_e mapstates[MAX_MAP_STATES];
} mapping_e;

void reset_circuit_mapping(void);
void reset_mapping_pointer(void);
void reset_synchro(void);
void check_synchro_start(void);
uint8_t is_hyper_turbo_safe(mapstate_e state);
mapping_e get_mapping_info(void);
void update_mapping_function(void);
uint8_t get_mapping_function(void);
uint16_t get_synchro_sector_idx(void);
uint16_t get_mapping_pointer_idx(void);
mapstate_e get_mapping_state(void);
mapstate_e get_synchro_state(void);
uint8_t get_end_of_mapping(void);
void get_next_sector(void);
int32_t get_largest_rect(void);
int32_t get_end_sector_largest_rect();
int32_t get_finish_mapping_largest_rect();
int32_t get_current_synchro_sector();

#endif
