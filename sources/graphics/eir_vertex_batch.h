#pragma once

#include "eir_gfx_types.h"
#include "eir_vertex.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_vertex_t, eir_gfx_vertex_array_t)

typedef struct
{
   eir_gfx_api_vao_handle_t vao;
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_vertex_array_t vertices;
   eir_gfx_primitive_type_t primitive_type;
   eir_gfx_api_program_handle_t program;
   bool built;
   bool modified;
} eir_gfx_vertex_batch_t;
