#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite.h"
#include "eir_texture.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_api_vao_handle_t vao;
   const eir_gfx_texture_t * texture;
   eir_gfx_api_program_handle_t program;
   eir_gfx_sprite_array_t sprites;
   bool built;
	bool modifiable;
   bool modified;
} eir_gfx_sprite_batch_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_batch_t, eir_gfx_sprite_batch_array_t);
