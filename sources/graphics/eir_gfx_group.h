#pragma once

#include "eir_gfx_sprite_info.h"
#include "eir_gfx_sprite_proxy.h"
#include "eir_gfx_sprite_batch.h"
#include "eir_gfx_text.h"
#include "eir_rect_batch.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_gfx_sprite_info_array_t sprite_infos;
   eir_gfx_text_info_array_t text_infos;
   eir_gfx_rect_info_array_t rect_infos;
   
   eir_gfx_sprite_proxy_array_t sprite_proxies;
   eir_gfx_text_proxy_array_t text_proxies
   eir_gfx_rect_proxy_array_t rect_proxies;

   eir_gfx_sprite_batch_array_t sprite_batches;
   eir_gfx_text_array_t texts;
   eir_gfx_rect_batch_array_t rect_batches;
   
   bool visible;
} eir_gfx_group_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_group_t, eir_gfx_group_array_t)
