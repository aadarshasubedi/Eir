#pragma once

#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   float weight;
} eir_gme_physic_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_physic_component_t,
	eir_gme_physic_component_array_t
	);

typedef struct
{
	int entity_index;
	float weight;
} eir_gme_physic_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_physic_info_t,
	eir_gme_physic_info_array_t
	);

#define EIR_GME_PARSE_physic(buffer, item, bytes_read) \
   sscanf(                                            \
      buffer,                                         \
      "%d|%f|%n",                                     \
      &item.entity_index,                             \
      &item.weight,                                   \
      &bytes_read                                     \
      );
