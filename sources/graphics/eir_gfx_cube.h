#pragma once

#include "../maths/eir_mth_vector.h"
#include "eir_gfx_color.h"
#include "../kernel/eir_ker_array_macro.h"

#define EIR_GFX_VERTICES_CUBE_COUNT 8
#define EIR_GFX_INDICES_CUBE_COUNT 32

typedef struct
{
   eir_mth_vec3_t vertices[EIR_GFX_VERTICES_CUBE_COUNT];
   int indices[EIR_GFX_INDICES_CUBE_COUNT];
   eir_gfx_color_t color;
   void * batch;
} eir_gfx_cube_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_cube_t, eir_gfx_cube_array_t);
