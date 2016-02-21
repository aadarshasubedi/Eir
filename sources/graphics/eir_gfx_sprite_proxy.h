#pragma once

#include "eir_gfx_sprite.h"
#include "eir_gfx_sprite_info.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_gfx_sprite_t * sprite;
   eir_gfx_sprite_info_t * sprite_info;
} eir_gfx_sprite_proxy_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_proxy_t, eir_gfx_sprite_proxy_array_t);
