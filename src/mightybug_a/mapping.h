#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "encoder.h"
#include <stdint.h>

/* Mapping variables */
#define FLAG_CIRCUIT_MAPPING 1
#define MAX_MAP_STATES 200
#define OUT_MAPSTLINE_STATE 6 // diff in abs encoder ticks
#define MIN_SECTOR_LENGTH 250 // minimum length of sector
#define SYNCHRO_MAX_DRIFT 200
#define TURBO_SYNCHRO_TICKS 300 // remaining ticks

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
uint8_t is_increase_vel_enable(mapstate_e state);
mapping_e get_mapping_info(void);
void update_mapping_function(void);
uint8_t get_mapping_function(void);
uint16_t get_synchro_sector_idx(void);
uint16_t get_mapping_pointer_idx(void);
mapstate_e get_mapping_state(void);
mapstate_e get_synchro_state(void);

#endif
