#pragma once

#include "../kernel/eir_ker_array_macro.h"
#include "../maths/eir_mth_vector.h"
#include "eir_gfx_color.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_color_t color;
   void * batch;
} eir_gfx_sprite_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_t, eir_gfx_sprite_array_t);
