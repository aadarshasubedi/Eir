#pragma once

#include "eir_gfx_sprite_batch.h"
#include "eir_gfx_text.h"
#include "eir_gfx_rect_batch.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_gfx_sprite_batch_array_t sprite_batches;
   eir_gfx_rect_batch_array_t rect_batches;
   eir_gfx_text_array_t texts;
   bool visible;
} eir_gfx_group_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_group_t, eir_gfx_group_array_t)
