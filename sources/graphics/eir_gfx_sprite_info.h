#pragma once

#include "../maths/eir_vector.h"
#include "eir_gfx_texture.h"
#include "eir_gfx_color.h"
#include "eir_gfx_types.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_texture_t * texture;
   eir_gfx_color_t color;
   eir_gfx_item_into_t info;
} eir_gfx_sprite_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_info_t, eir_gfx_sprite_info_array_t);
