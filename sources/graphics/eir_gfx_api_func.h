#pragma once

#include "eir_gfx_env.h"
#include "eir_gfx_types.h"
#include "eir_gfx_sprite_batch.h"
#include "eir_gfx_image.h"

void eir_gfx_api_init();
void eir_gfx_api_release();

eir_gfx_api_texture_handle_t eir_gfx_api_create_texture(const eir_gfx_image_t * image);

void eir_gfx_api_bind_texture(eir_gfx_api_texture_handle_t texture_handle);
void eir_gfx_api_bind_vertex_array(eir_gfx_api_vao_handle_t vao_handle);
void eir_gfx_api_bind_array_buffer(eir_gfx_api_vbo_handle_t vbo_handle);
void eir_gfx_api_unbind_vertex_array();
void eir_gfx_api_use_program(eir_gfx_api_program_handle_t program_handle);
void eir_gfx_api_unuse_program();
void eir_gfx_api_set_program_mat4(const char * var_name, eir_gfx_api_program_handle_t program_handle, const eir_mth_mat4_t * m);

void eir_gfx_api_load_sprite_shaders(eir_gfx_env_t * gfx_env);
void eir_gfx_api_load_text_shaders(eir_gfx_env_t * gfx_env);
void eir_gfx_api_load_rect_shaders(eir_gfx_env_t * gfx_env);
void eir_gfx_api_compile_shader(
   eir_gfx_api_shader_handle_t shader,
   const char * filename
   );

void eir_gfx_api_set_sprite_buffer_data(eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_set_rect_buffer_data(eir_gfx_rect_batch_t * batch);

void eir_gfx_api_build_sprite_batch(eir_gfx_env_t * gfx_env, eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_build_text_batch(eir_gfx_env_t * gfx_env, eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_build_rect_batch(eir_gfx_env_t * gfx_env, eir_gfx_rect_batch_t * batch);

void eir_gfx_api_draw_sprite_batch(eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_draw_rect_batch(eir_gfx_rect_batch_t * batch);

void eir_gfx_api_set_clear_color();
void eir_gfx_api_clear_buffer();
void eir_gfx_api_bind_sprite_attributes(
   eir_gfx_api_att_handle_t pos_attr,
   eir_gfx_api_att_handle_t size_attr,
   eir_gfx_api_att_handle_t uv_offset_attr,
   eir_gfx_api_att_handle_t uv_size_attr,
   eir_gfx_api_att_handle_t color_attr
   );
void eir_gfx_api_bind_rect_attributes(
   eir_gfx_api_att_handle_t pos_attr,
   eir_gfx_api_att_handle_t size_attr,
   eir_gfx_api_att_handle_t color_attr
   );
void eir_gfx_api_destroy_texture(eir_gfx_api_texture_handle_t texture_handle);
void eir_gfx_api_release_sprite_batch(eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_release_rect_batch(eir_gfx_rect_batch_t * batch);
void eir_gfx_api_unload_sprite_shaders(eir_gfx_env_t * gfx_env);
void eir_gfx_api_unload_text_shaders(eir_gfx_env_t * gfx_env);
void eir_gfx_api_unload_rect_shaders(eir_gfx_env_t * gfx_env);
void eir_gfx_api_check_for_error();
