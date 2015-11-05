#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite_batch.h"

void eir_gfx_api_init();
void eir_gfx_api_compile_shader(eir_gfx_api_shader_handle_t shader, const char * filename);
void eir_gfx_api_set_buffer_data(eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_build_sprite_batch(eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_draw_sprite_batch(eir_gfx_sprite_batch_t * batch);
void eir_gfx_api_set_clear_color();
void eir_gfx_api_clear_buffer();
void eir_gfx_api_bind_sprite_attributes(
   eir_gfx_api_att_handle_t pos_attr,
   eir_gfx_api_att_handle_t size_attr,
   eir_gfx_api_att_handle_t uv_offset_attr,
   eir_gfx_api_att_handle_t uv_size_attr,
   eir_gfx_api_att_handle_t color_attr
   );
void eir_gfx_api_release_batch(eir_gfx_sprite_batch_t * batch);
