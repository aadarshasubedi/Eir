#pragma once

#include "../maths/eir_mth_vector.h"
#include "eir_gfx_color.h"
#include "eir_gfx_types.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_gfx_color_t color;
   eir_gfx_item_info_t info;
} eir_gfx_rect_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_rect_info_t, eir_gfx_rect_info_array_t)
