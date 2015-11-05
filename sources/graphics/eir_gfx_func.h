#pragma once

#include "eir_gfx_env.h"
#include "eir_sprite_batch.h"

void eir_gfx_set_sprite_batch_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
eir_gfx_sprite_batch_t * eir_gfx_create_empty_sprite_batch(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_add_sprite_to_batch(
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   eir_gfx_color_t * color,
   eir_gfx_sprite_batch_t * batch
   );
void eir_gfx_render_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_release_all_batches(eir_gfx_env_t * gfx_env);
