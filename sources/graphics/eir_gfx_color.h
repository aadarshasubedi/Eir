#pragma once

#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   float r;
   float g;
   float b;
   float a;
} eir_gfx_color_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_color_t, eir_gfx_color_array_t);
