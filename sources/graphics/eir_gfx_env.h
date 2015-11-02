#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite_batch.h"

typedef struct
{
   eir_gfx_window_handle_t window;
   eir_gfx_context_handle_t context;
   eir_gfx_sprite_batch_t sprite_batch;
} eir_gfx_env_t;
