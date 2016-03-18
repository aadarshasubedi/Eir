#pragma once

#include "../graphics/eir_gfx_sprite_proxy.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_gfx_sprite_proxy_t * sprite_proxy;
} eir_gme_sprite_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_sprite_component_t,
   eir_gme_sprite_component_array_t
   );

typedef struct
{
	int entity_index;
	int texture_index;
	float x_uv_offset;
	float y_uv_offset;
	float x_uv_size;
	float y_uv_size;
} eir_gme_sprite_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_sprite_info_t,
	eir_gme_sprite_info_array_t
	);

#define EIR_GME_PARSE_sprite(buffer, item, bytes_read)  \
   sscanf(                                            \
      buffer,                                         \
      "%d|%d|%f|%f|%f|%f|%n",                         \
      &item.entity_index,                             \
      &item.texture_index,                            \
      &item.x_uv_offset,                              \
      &item.y_uv_offset,                              \
      &item.x_uv_size,                                \
      &item.y_uv_size,                                \
      &bytes_read                                     \
      );
