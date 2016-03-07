#pragma once

#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   char * pixels;
   int width;
   int height;
} eir_gfx_image_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_image_t, eir_gfx_image_array_t);
