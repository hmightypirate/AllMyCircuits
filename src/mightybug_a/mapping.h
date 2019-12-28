#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "encoder.h"
#include <stdint.h>
#include <stdlib.h>

/* Mapping variables */
#define FLAG_CIRCUIT_MAPPING 1
#define MAX_MAP_STATES 200
#define OUT_MAPSTLINE_STATE 9 // diff in abs encoder ticks
#define MIN_SECTOR_LENGTH 180 // minimum length of sector
#define SYNCHRO_MAX_DRIFT 200
#define TURBO_SYNCHRO_TICKS 400 // remaining ticks

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
} sector_type_e;

typedef struct {
	int32_t agg_total_ticks[MAX_MAP_STATES];
	int32_t agg_left_ticks[MAX_MAP_STATES];
	int32_t agg_right_ticks[MAX_MAP_STATES];
	int32_t first_tick[MAX_MAP_STATES];
	sector_type_e mapstates[MAX_MAP_STATES];
} mapping_e;

#define NO_MAPMODE 0
#define LINEAR_MAPMODE 1
#define CIRCULAR_MAPMODE 2

void reset_circuit_mapping(void);
void reset_mapping_pointer(void);
void reset_synchro(void);
void update_synchro_mapping_flag(void);
uint8_t is_hyper_turbo_safe(sector_type_e state);
mapping_e get_mapping_info(void);
void update_mapping(void);
uint8_t get_synchro_flag(void);
uint16_t get_synchro_sector_idx(void);
uint16_t get_mapping_pointer_idx(void);
sector_type_e get_mapping_state(void);
sector_type_e get_synchro_state(void);
uint8_t get_end_of_linear_mapping_run(void);
void get_next_sector(void);
int32_t get_largest_st_line_size(void);
int32_t get_start_loop_sector();
int32_t get_end_loop_sector();

void next_mapmode(void);
uint8_t get_mapmode(void);

void next_mapmode(void);
uint8_t get_mapmode(void);

#endif
