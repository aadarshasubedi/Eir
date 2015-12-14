#pragma once

#include "../maths/eir_vector.h"
#include "eir_texture.h"

typedef struct
{
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_texture_t * texture;
} eir_gfx_sprite_ref_t;
