#pragma once

#include "eir_gfx_sprite.h"
#include "eir_gfx_texture.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_texture_t * texture;
   eir_gfx_color_t color;
   eir_gfx_item_info_t info;
   void * buffer;
} eir_gfx_sprite_proxy_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_proxy_t, eir_gfx_sprite_proxy_array_t);
