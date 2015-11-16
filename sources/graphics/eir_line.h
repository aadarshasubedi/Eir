#pragma once 

#include "../maths/eir_vector.h"
#include "eir_color.h"

typedef struct
{
   eir_mth_vec2_t a;
   eir_mth_vec2_t b;
   eir_gfx_color_t a_color;
   eir_gfx_color_t b_color;
} eir_gfx_line_t;
