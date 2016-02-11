#pragma once
#include "eir_gfx_env.h"
//#include "eir_sprite_batch.h"
//#include "../game/eir_world.h"

void eir_gfx_init_env(eir_gfx_env_t * gfx_env, int width, int height);
void eir_gfx_release_env(eir_gfx_env_t * gfx_env);

void eir_gfx_set_image_capacity(eir_gfx_env_t * env, size_t max_count);
void eir_gfx_set_texture_capacity(eir_gfx_env_t * env, size_t max_count);
//void eir_gfx_set_sprite_ref_capacity(eir_gfx_env_t * env, size_t max_count);
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
/*
eir_gfx_sprite_ref_t * eir_gfx_create_sprite_ref(
	eir_gfx_env_t * env,
	const eir_gfx_texture_t * texture,
	int img_x_offset,
	int img_y_offset,
	int img_width_offset,
	int img_height_offset
	);
*/
eir_gfx_group_t * eir_gfx_create_group(
	eir_gfx_env_t * env,
	size_t sprite_batch_capacity,
	size_t text_batch_capacity,
	size_t rect_capacity
	);
eir_gfx_sprite_t * eir_gfx_push_sprite_to_group(
	eir_gfx_group_t * group,
	const eir_mth_vec2_t * position,
	const eir_mth_vec2_t * size,
	const eir_mth_vec2_t * uv_offset,
	const eir_mth_vec2_t * uv_size,
	const eir_gfx_color_t * color
	);
eir_gfx_text_t * eir_gfx_push_text_to_group(
	eir_gfx_group_t * group,
	const char * text,
	const eir_mth_vec2_t * position,
	float font_size,
	const eir_gfx_color_t * color
	);
eir_gfx_rect_t * eir_gfx_push_rect_to_group(
	eir_gfx_group_t * group,
	const eir_mth_vec2_t * position,
	const eir_mth_vec2_t * size,
	const eir_gfx_color_t * color
	);
void eir_gfx_set_group_visibility(
	eir_gfx_group_t * group,
	bool visible
	);
void eir_gfx_modify_sprite(
	eir_gfx_sprite_t * gfx_sprite,
	const eir_mth_vec2_t * position,
	const eir_mth_vec2_t * size,
	const eir_mth_vec2_t * uv_offset,
	const eir_mth_vec2_t * uv_size,
	const eir_gfx_color_t * color
	);
void eir_gfx_modify_text(
	eir_gfx_text_t * gfx_text,
	const char * text,
	const eir_mth_vec2_t * positions,
	float font_size,
	const eir_gfx_color_t * color
	);
void eir_gfx_modify_rect(
	eir_gfx_rect_t * gfx_rect,
	const eir_mth_vec2_t * position,
	const eir_mth_vec2_t * size,
	const eir_gfx_color_t * color
	);
void eir_gfx_set_sprite_visibility(
	eir_gfx_sprite_t * gfx_sprite,
	bool visible
	);
void eir_gfx_set_text_visibility(
	eir_gfx_text_t * gfx_text,
	bool visible
	);
void eir_gfx_set_rect_visibility(
	eir_gfx_rect_t * rect,
	bool visible
	);

/*
void eir_gfx_set_batch_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_set_text_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
void eir_gfx_set_rect_capacity(eir_gfx_env_t * gfx_env, int max_capacity);
int eir_gfx_add_text(eir_gfx_env_t * gfx_env, const char * text, eir_mth_vec2_t * position, float font_size, eir_gfx_color_t * color);
void eir_gfx_generate_all_batches(eir_gfx_env_t * gfx_env, const eir_gme_world_t * world);
*/

//void eir_gfx_force_update_all_batches(eir_gfx_env_t * gfx_env);
void eir_gfx_render_all_batches(eir_gfx_env_t * gfx_env);
//void eir_gfx_update_text(eir_gfx_env_t * gfx_env, int text_index, const char * text);
void eir_gfx_update_camera_view(eir_gfx_env_t * gfx_env, const eir_mth_vec2_t * cam_pos);
