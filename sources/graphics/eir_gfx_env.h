#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite_batch.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_batch_t, eir_gfx_sprite_batch_array_t);

typedef struct
{
   eir_gfx_window_handle_t window;
   eir_gfx_context_handle_t context;
   eir_gfx_sprite_batch_array_t sprite_batches;
} eir_gfx_env_t;
