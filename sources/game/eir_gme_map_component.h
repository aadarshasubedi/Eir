#pragma once

#include "../maths/eir_mth_vector.h"
#include "../kernel/eir_ker_array_macro.h"
#include "../graphics/eir_gfx_group.h"
#include "../graphics/eir_gfx_sprite_batch.h"

typedef struct
{
   int col_index;
   int row_index;
} eir_gme_map_tile_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_map_tile_t,
   eir_gme_map_tile_array_t
   );

typedef struct
{
   eir_mth_vec2_t position;
   int col_count;
   int row_count;
   int tile_width;
   int tile_height;
   eir_gfx_group_t * group;
   eir_gfx_sprite_batch_t * batch;
   eir_gme_map_tile_array_t tiles;
} eir_gme_map_layer_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_map_layer_t,
   eir_gme_map_layer_array_t
   );

typedef struct
{
   eir_gme_map_layer_array_t layers;
} eir_gme_map_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_map_component_t,
   eir_gme_map_component_array_t
   );
