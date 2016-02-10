#pragma once

#include "../maths/eir_vector.h"
#include "eir_color.h"
#include "../kernel/eir_array_macro.h"

struct eir_gfx_sprite_batch_t;

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_color_t color;
	struct eir_gfx_sprite_batch_t * batch;
	bool visible;
} eir_gfx_sprite_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_t, eir_gfx_sprite_array_t);
