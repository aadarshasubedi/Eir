#pragma once

#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	bool visible;
	bool alive;
} eir_gme_state_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_state_component_t,
	eir_gme_state_component_array_t
	);

typedef struct
{
	int entity_index;
	int alive;
	int visible;
} eir_gme_state_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_state_info_t,
	eir_gme_state_info_array_t
	);

#define EIR_GME_PARSE_state(buffer, item, bytes_read) \
   sscanf(                                            \
      buffer,                                         \
      "%d|%d|%d|%n",                                  \
      &item.entity_index,                             \
      &item.alive,                                    \
      &item.visible,                                  \
      &bytes_read                                     \
      );
