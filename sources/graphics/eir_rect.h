#pragma once

#include "../maths/eir_vector.h"
#include "eir_color.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_gfx_color_t color;
} eir_gfx_rect_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_rect_t, eir_gfx_rect_array_t)
