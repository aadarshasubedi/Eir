#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite.h"

// TODO: to remove use dynamic array for sprites
#define MAX_SPRITES_PER_BATCH 10

typedef struct
{
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_api_vao_handle_t vao;
   eir_gfx_api_texture_handle_t texture[1];
   eir_gfx_api_shader_handle_t vertex_shader;
   eir_gfx_api_shader_handle_t geometry_shader;
   eir_gfx_api_shader_handle_t fragment_shader;
   eir_gfx_api_program_handle_t program;
   eir_gfx_sprite_t sprites[MAX_SPRITES_PER_BATCH]; // TODO: use dynamic array for sprites
   size_t curr_sprites_count;
} eir_gfx_sprite_batch_t;
