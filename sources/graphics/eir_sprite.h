#pragma once

#include "../maths/eir_vector.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
} eir_gfx_sprite_t;
