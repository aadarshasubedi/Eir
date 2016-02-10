#pragma once

#include "eir_gme_env.h"

void eir_gme_init_env(eir_gme_env_t * env);
void eir_gme_world_entity_update_linked_components(eir_gme_world_t * world);
void eir_gme_release_env(eir_gme_env_t * env);
void eir_gme_set_world_capacity(eir_gme_env_t * env, size_t max_count);

eir_gme_world_t * eir_gme_create_world(
	eir_gme_env_t * env,
	size_t max_entity_count
	);

eir_gme_entity_t eir_gme_create_world_entity(eir_gme_world_t * world);

eir_gme_position_component_t * eir_gme_set_entity_position(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	int x,
	int y
	);

eir_gme_size_component_t * eir_gme_set_entity_size(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	int width,
	int height
	);

eir_gme_sprite_ref_component_t * eir_gme_set_entity_sprite_ref(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_gfx_sprite_ref_t * sprite_ref
	);

eir_gme_color_component_t * eir_gme_set_entity_color(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float r,
	float g,
	float b,
	float a
	);

eir_gme_motion_param_component_t * eir_gme_set_entity_acceleration(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float ax,
	float ay,
	float speed,
	float friction
	);

eir_gme_aabb_component_t * eir_gme_set_entity_aabb(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float x,
	float y,
	float width,
	float height
	);

eir_gme_camera_component_t * eir_gme_set_entity_camera(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float win_scale,
	int viewport_w,
	int viewport_h
	);

eir_gme_physic_component_t * eir_gme_set_entity_physic(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float weight
	);

eir_gme_direction_component_t * eir_gme_set_entity_direction(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	eir_gme_direction_t direction
	);

eir_gme_based_melee_attack_component_t * eir_gme_set_entity_based_melee_attack(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	float damage,
	float damage_zone_x,
	float damage_zone_y,
	float damage_zone_width,
	float damage_zone_height,
	bool active
	);

eir_gme_state_component_t * eir_gme_set_entity_state(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	bool visible,
	bool alive
	);

void eir_gme_set_active_camera(
	eir_gme_world_t * world,
	eir_gme_entity_t entity
	);

void eir_gme_set_active_world(
	eir_gme_env_t * env,
	eir_gme_world_t * world
	);
