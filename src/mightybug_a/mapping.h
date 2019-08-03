#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "encoder.h"
#include <stdint.h>

/* Mapping variables */
#define FLAG_CIRCUIT_MAPPING 1
#define MAX_MAP_STATES 200
#define OUT_MAPSTLINE_STATE 4 // diff in abs encoder ticks
#define MIN_SECTOR_LENGTH 250 // minimum length of sector
#define SYNCHRO_MAX_DRIFT 200

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
void do_circuit_mapping(void);
void do_synchro_run(void);

mapping_e get_mapping_info(void);

#endif
