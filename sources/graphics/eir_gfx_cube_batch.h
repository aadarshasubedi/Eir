#pragma once

#include "eir_gfx_types.h"
#include "eir_gfx_cube.h"
#include "eir_gfx_cube_proxy.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_api_vao_handle_t vao;
   eir_gfx_api_program_handle_t program;
   eir_gfx_cube_proxy_array_t cubes_proxies;
   eir_gfx_cube_array_t cubes;
   eir_gfx_item_info_t info;
} eir_gfx_cube_batch_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gfx_cube_batch_t,
   eir_gfx_cube_batch_array_t
   );
