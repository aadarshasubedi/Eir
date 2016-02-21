#pragma once

#include "../maths/eir_mth_vector.h"
#include "eir_gfx_color.h"
#include "eir_gfx_types.h"
#include "../kernel/eir_ker_array_macro.h"

#define EIR_GFX_TEXT_INFO_BUFFER 256

typedef struct
{
   char text[256];
   eir_mth_vec2_t position;
   float font_size;
   eir_gfx_color_t color;
   eir_gfx_item_info_t info;
} eir_gfx_text_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_text_info_t, eir_gfx_text_info_array_t)
