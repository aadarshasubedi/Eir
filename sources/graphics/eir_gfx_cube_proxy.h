#pragma once

#include "../maths/eir_mth_vector.h"
#include "eir_gfx_color.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_mth_vec3_t position;
   eir_mth_vec3_t size;
   eir_gfx_color_t color;
   void * buffer;
} eir_gfx_cube_proxy_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_cube_proxy_t, eir_gfx_cube_proxy_array_t);
