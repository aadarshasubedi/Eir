#pragma once
#include "eir_gfx_types.h"
#include "eir_sprite.h"
#include "eir_texture.h"

typedef struct
{
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_api_vao_handle_t vao;
   eir_gfx_texture_t * texture;
   eir_gfx_api_program_handle_t program;
   eir_gfx_sprite_array_t sprites;
   bool built;
   bool modified;
} eir_gfx_sprite_batch_t;
