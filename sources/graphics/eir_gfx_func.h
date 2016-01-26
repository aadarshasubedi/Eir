#pragma once
#include "../eir.h"
#include "eir_gfx_env.h"
#include "eir_sprite_batch.h"
#include "../game/eir_world.h"

void eir_gfx_init_env(eir_gfx_env_t * gfx_env, int width, int height);
void eir_gfx_release_env(eir_gfx_env_t * gfx_env);
void eir_gfx_set_batch_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_set_text_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_set_rect_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
eir_handle_t eir_gfx_add_text(
   eir_gfx_env_t * gfx_env,
   const char * text,
   eir_mth_vec2_t * position,
   float font_size,
   eir_gfx_color_t * color
   );
void eir_gfx_generate_all_batches(eir_gfx_env_t * gfx_env, const eir_gme_world_t * world);
void eir_gfx_force_update_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_render_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_update_text(eir_gfx_env_t * gfx_env, eir_handle_t text_handle, const char * text);
void eir_gfx_update_camera_view(eir_gfx_env_t * gfx_env, const eir_mth_vec2_t * cam_pos);
