#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite_batch.h"
#include "eir_vertex_batch.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_batch_t, eir_gfx_sprite_batch_array_t);

typedef struct
{
   eir_gfx_window_handle_t window;
   eir_gfx_context_handle_t context;
   
   eir_gfx_sprite_batch_array_t sprite_batches;
   eir_gfx_sprite_batch_array_t text_batches;
   eir_gfx_vertex_batch_t line_batch;
   eir_gfx_vertex_batch_t quad_batch;
   
   eir_gfx_api_shader_handle_t sprite_vert_shader;
   eir_gfx_api_shader_handle_t sprite_geom_shader;
   eir_gfx_api_shader_handle_t sprite_frag_shader;
   eir_gfx_api_program_handle_t sprite_program;
   
   eir_gfx_api_shader_handle_t text_vert_shader;
   eir_gfx_api_shader_handle_t text_geom_shader;
   eir_gfx_api_shader_handle_t text_frag_shader;
   eir_gfx_api_program_handle_t text_program;
   
   eir_gfx_api_shader_handle_t default_vert_shader;
   eir_gfx_api_shader_handle_t default_frag_shader;
   eir_gfx_api_program_handle_t default_program;
} eir_gfx_env_t;
