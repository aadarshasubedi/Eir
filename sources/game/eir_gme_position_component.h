#pragma once

#include "../maths/eir_mth_vector.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_mth_vec2_t position;
	bool modified;
} eir_gme_position_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_position_component_t,
   eir_gme_position_component_array_t
   );

typedef struct
{
	int entity_index;
	float x;
	float y;
} eir_gme_position_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_position_info_t,
	eir_gme_position_info_array_t
	);

#define EIR_GME_PARSE_position(buffer, item, bytes_read) \
   sscanf(                                            \
      buffer,                                         \
      "%d|%f|%f|%n",                                  \
      &item.entity_index,                             \
      &item.x,                                        \
      &item.y,                                        \
      &bytes_read                                     \
      );
