#pragma once

#include "eir_gfx_types.h"
#include "eir_point.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_point_t, eir_gfx_point_array_t)

typedef struct
{
   eir_gfx_api_vao_handle_t vao;
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_point_array_t points;
   bool built;
   bool modified;
} eir_gfx_line_batch_t;
