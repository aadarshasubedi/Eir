#pragma once

#include "../eir.h"
#include "eir_gfx_env.h"
#include "eir_sprite_batch.h"

void eir_gfx_set_batch_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_set_text_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_set_line_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_set_quad_capacity(eir_gfx_env_t * gfx_env, int max_capacity);

eir_handle_t eir_gfx_create_sprite_batch(eir_gfx_env_t * gfx_env, int max_capacity);

eir_handle_t eir_gfx_add_sprite(
   eir_gfx_env_t * gfx_env,
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   eir_gfx_color_t * color,
   eir_handle_t batch_handle
   );
eir_handle_t eir_gfx_add_text(
   eir_gfx_env_t * gfx_env,
   const char * text,
   eir_mth_vec2_t * position,
   float font_size,
   eir_gfx_color_t * color
   );
eir_handle_t eir_gfx_add_line(
   eir_gfx_env_t * gfx_env,
   eir_mth_vec2_t * a,
   eir_mth_vec2_t * b,
   eir_gfx_color_t * a_color,
   eir_gfx_color_t * b_color
   );
eir_handle_t eir_gfx_add_quad(
   eir_gfx_env_t * gfx_env,
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_gfx_color_t * color
   );

void eir_gfx_create_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_render_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_update_text(eir_gfx_env_t * gfx_env, eir_handle_t text_handle, const char * text);
void eir_gfx_release_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_release_all_images(eir_gfx_env_t * gfx_env);
void eir_gfx_release_all_sprites_ref(eir_gfx_env_t * gfx_env);
void eir_gfx_release_all_textures(eir_gfx_env_t * gfx_env);
