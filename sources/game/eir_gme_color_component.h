#pragma once

#include "../graphics/eir_gfx_color.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_gfx_color_t color;
	bool modified;
} eir_gme_color_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_color_component_t,
	eir_gme_color_component_array_t
	);

typedef struct
{
	int entity_index;
	float r;
	float g;
	float b;
	float a;
} eir_gme_color_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_color_info_t,
	eir_gme_color_info_array_t
	);

#define EIR_GME_PARSE_color(buffer, item, bytes_read) \
   sscanf(                                            \
      buffer,                                         \
      "%d|%f|%f|%f|%f|%n",                            \
      &item.entity_index,                             \
      &item.r,                                        \
      &item.g,                                        \
      &item.b,                                        \
      &item.a,                                        \
      &bytes_read                                     \
      );
