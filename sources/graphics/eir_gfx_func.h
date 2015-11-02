#pragma once

#include "eir_sprite_batch.h"

void eir_gfx_add_sprite_to_batch(
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   eir_gfx_sprite_batch_t * batch
   );
