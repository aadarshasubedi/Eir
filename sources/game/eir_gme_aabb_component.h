#pragma once

#include "../physics/eir_phy_aabb.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_phy_aabb_t aabb;
   float x_offset;
   float y_offset;
   bool modified;
} eir_gme_aabb_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_aabb_component_t,
   eir_gme_aabb_component_array_t
   );

typedef struct
{
	int entity_index;
	float x_offset;
	float y_offset;
	float width;
	float height;
} eir_gme_aabb_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_aabb_info_t,
	eir_gme_aabb_info_array_t
	);

#define EIR_GME_PARSE_aabb(buffer, item, bytes_read)  \
   sscanf(                                            \
      buffer,                                         \
      "%d|%f|%f|%f|%f|%n",                            \
      &item.entity_index,                             \
      &item.x_offset,                                 \
      &item.y_offset,                                 \
      &item.width,                                    \
      &item.height,                                   \
      &bytes_read                                     \
      );
