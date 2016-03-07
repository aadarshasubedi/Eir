#pragma once

#include "eir_gfx_types.h"
#include "eir_gfx_sprite.h"
#include "eir_gfx_texture.h"
#include "eir_gfx_sprite_proxy.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_api_vao_handle_t vao;
   const eir_gfx_texture_t * texture;
   eir_gfx_api_program_handle_t program;
   eir_gfx_sprite_proxy_array_t sprites_proxies;
   eir_gfx_sprite_array_t sprites;
   eir_gfx_item_info_t info;
} eir_gfx_sprite_batch_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gfx_sprite_batch_t,
   eir_gfx_sprite_batch_array_t
   );
