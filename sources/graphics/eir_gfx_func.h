#pragma once
#include "eir_gfx_env.h"

void eir_gfx_init_env(eir_gfx_env_t * gfx_env, int width, int height);
void eir_gfx_release_env(eir_gfx_env_t * gfx_env);

void eir_gfx_set_image_capacity(eir_gfx_env_t * env, size_t max_count);
void eir_gfx_set_texture_capacity(eir_gfx_env_t * env, size_t max_count);
void eir_gfx_set_group_capacity(eir_gfx_env_t * env, size_t max_count);

eir_gfx_image_t * eir_gfx_load_image(
   eir_gfx_env_t * env,
   const char * img_filename,
   bool must_invert_img
   );
eir_gfx_texture_t * eir_gfx_create_texture(
   eir_gfx_env_t * env,
   const eir_gfx_image_t * image
   );
eir_gfx_group_t * eir_gfx_create_group(
   eir_gfx_env_t * env,
   size_t sprite_capacity,
   size_t text_capacity,
   size_t rect_capacity
   );
eir_gfx_sprite_proxy_t * eir_gfx_push_sprite_to_group(
   eir_gfx_group_t * group,
   const eir_gfx_texture_t * texture,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_mth_vec2_t * uv_offset,
   const eir_mth_vec2_t * uv_size,
   const eir_gfx_color_t * color,
   bool editable,
   bool use_screen_coord,
   bool visible
   );
eir_gfx_text_proxy_t * eir_gfx_push_text_to_group(
   eir_gfx_group_t * group,
   const char * text,
   const eir_mth_vec2_t * position,
   float font_size,
   const eir_gfx_color_t * color,
   bool editable,
   bool use_screen_coord,
   bool visible
   );
eir_gfx_rect_proxy_t * eir_gfx_push_rect_to_group(
   eir_gfx_group_t * group,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_gfx_color_t * color,
   bool editable,
   bool use_screen_coord,
   bool visible
   );
void eir_gfx_set_group_visibility(
   eir_gfx_group_t * group,
   bool visible
   );
void eir_gfx_modify_sprite(
   eir_gfx_sprite_proxy_t * gfx_sprite_proxy,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_mth_vec2_t * uv_offset,
   const eir_mth_vec2_t * uv_size,
   const eir_gfx_color_t * color
   );
void eir_gfx_modify_text(
   eir_gfx_text_proxy_t * gfx_text_proxy,
   const char * text,
   const eir_mth_vec2_t * positions,
   float font_size,
   const eir_gfx_color_t * color
   );
void eir_gfx_modify_rect(
   eir_gfx_rect_proxy_t * gfx_rect_proxy,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_gfx_color_t * color
   );
void eir_gfx_set_sprite_visibility(
   eir_gfx_sprite_proxy_t * gfx_sprite_proxy,
   bool visible
   );
void eir_gfx_set_text_visibility(
   eir_gfx_text_proxy_t * gfx_text_proxy,
   bool visible
   );
void eir_gfx_set_rect_visibility(
   eir_gfx_rect_proxy_t * gfx_rect_proxy,
   bool visible
   );
void eir_gfx_render_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_update_camera_view(
   eir_gfx_env_t * gfx_env,
   const eir_mth_vec2_t * cam_pos
   );
