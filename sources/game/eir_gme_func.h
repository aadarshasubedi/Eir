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

void eir_gme_set_entity_based_melee_attack(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float damage,
	float damage_zone_x,
	float damage_zone_y,
	float damage_zone_width,
	float damage_zone_height,
	bool active
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
