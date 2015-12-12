#pragma once

#include "../maths/eir_vector.h"
#include "eir_image.h"

typedef struct
{
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_image_t * image;
} eir_gfx_sprite_ref_t;
