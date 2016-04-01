#pragma once

#include "eir_gme_env.h"
#include "../graphics/eir_gfx_env.h"

void eir_gme_init_env(eir_gme_env_t * env);
void eir_gme_world_entity_update_linked_components(eir_gme_world_t * world);
void eir_gme_release_env(eir_gme_env_t * env);
void eir_gme_set_world_capacity(eir_gme_env_t * env, size_t max_count);

eir_gme_world_t * eir_gme_create_world(
	eir_gme_env_t * env,
	size_t max_entity_count
	);

eir_gme_entity_t eir_gme_create_world_entity(eir_gme_world_t * world);

void eir_gme_set_entity_position(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float x,
	float y
	);

void eir_gme_set_entity_size(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float width,
	float height
	);

void eir_gme_set_entity_sprite(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_gfx_sprite_proxy_t * sprite_proxy
	);

void eir_gme_set_entity_color(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float r,
	float g,
	float b,
	float a
	);

void eir_gme_set_entity_acceleration(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float ax,
	float ay,
	float speed,
	float friction
	);

void eir_gme_set_entity_aabb(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float x_offset,
	float y_offset,
	float width,
	float height
	);

void eir_gme_set_entity_physic(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float weight
	);

void eir_gme_set_entity_direction(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_gme_direction_t direction
	);

void eir_gme_set_entity_state(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	bool visible,
	bool alive
	);

void eir_gme_set_entity_fsm(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_fsm_state_machine_t * fsm
	);

void eir_gme_set_entity_aabb_primitive(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_gfx_rect_proxy_t * rect_proxy
	);

void eir_gme_set_entity_keyboard_controller(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_gme_input_controller_buffer_t * input_buffer
	);

void eir_gme_set_entity_pad_controller(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_gme_input_controller_buffer_t * input_buffer
	);

void eir_gme_set_entity_map(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   size_t layers_capacity
   );

void eir_gme_set_entity_map_layer(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gfx_group_t * group,
   eir_gfx_sprite_batch_t * batch,
   const eir_mth_vec2_t * position,
   int col_count,
   int row_count,
   int tile_width,
   int tile_height,
   size_t tiles_capacity
   );

void eir_gme_set_entity_map_tile(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   int layer_index,
   int col_index,
   int row_index,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   bool navigable
   );

void eir_gme_set_entity_map_layer_link(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gme_entity_t map_entity,
   int map_layer_index
   );

void eir_gme_set_entity_colliding_map_tiles(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   size_t max_tiles_capacity
   );

void eir_gme_set_active_camera(
	eir_gme_world_t * world,
	eir_gme_entity_t target,
	float win_scale,
	int viewport_w,
	int viewport_h
	);

void eir_gme_set_active_world(
	eir_gme_env_t * env,
	eir_gme_world_t * world
	);
