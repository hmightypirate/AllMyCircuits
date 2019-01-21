#ifndef __MAPPING_H__
#define __MAPPING_H__

#include <stdint.h>
#include "encoder.h"

/* Mapping variables */
#define FLAG_CIRCUIT_MAPPING 1
#define MAX_MAP_STATES 100
#define MIN_TICKS_FOR_MAP 400
#define DIFF_TICKS_EQUAL 100
#define OUT_MAPCORNER_STATE 2  // diff in abs encoder ticks
#define OUT_MAPSTLINE_STATE 4  // diff in abs encoder ticks 
#define FLAG_MAPPING_REPS 0 // mapping with repetitions

typedef enum {
  NONE,
  LEFT_CORNER,
  RIGHT_CORNER,
  ST_LINE,
  MAX_MAP_EVENTS
} mapstate_e;

typedef struct {
  int32_t agg_left_ticks[MAX_MAP_STATES];
  int32_t agg_right_ticks[MAX_MAP_STATES];
  mapstate_e mapstates[MAX_MAP_STATES];
  int16_t large_stline_pointer;
  int16_t rep_pointer;
  
} mapping_e;

void reset_circuit_mapping(void);
void reset_mapping_pointer(void);
void do_circuit_mapping(void);
mapping_e get_mapping_info(void);

#endif