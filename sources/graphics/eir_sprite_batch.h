#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_t, eir_gfx_sprite_array_t);

typedef struct
{
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_api_vao_handle_t vao;
   eir_gfx_api_texture_handle_t texture[1];
   eir_gfx_api_shader_handle_t vertex_shader;
   eir_gfx_api_shader_handle_t geometry_shader;
   eir_gfx_api_shader_handle_t fragment_shader;
   eir_gfx_api_program_handle_t program;
   eir_gfx_sprite_array_t sprites;
   bool built;
   bool modified;
} eir_gfx_sprite_batch_t;
