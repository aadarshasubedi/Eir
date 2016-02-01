#pragma once

#include "eir_gme_env.h"

void eir_gme_init_env(eir_gme_env_t * env);
void eir_gme_world_entity_update_linked_components(eir_gme_world_t * world);
void eir_gme_release_env(eir_gme_env_t * env);

void eir_gme_set_max_world_count(eir_gme_env_t * env, size_t max_count);
eir_gme_world_t * eir_gme_create_world(eir_gme_env_t * env, size_t max_entity_count);
void eir_gme_set_curr_world(eir_gme_env_t * env, eir_gme_world_t * world);
eir_gme_entity_t eir_gme_create_world_entity(eir_gme_world_t * world);

bool eir_gme_set_entity_position(eir_gme_world_t * world, eir_gme_entity_t entity, int x, int y);
bool eir_gme_set_entity_size(eir_gme_world_t * world, eir_gme_entity_t entity, int width, int height);
bool eir_gme_set_entity_sprite_ref(eir_gme_world_t * world, eir_gme_entity_t entity, eir_gfx_sprite_ref_t * sprite_ref);
bool eir_gme_set_entity_color(eir_gme_world_t * world, eir_gme_entity_t entity, float r, float g, float b, float a);
bool eir_gme_set_entity_acceleration(eir_gme_world_t * world, eir_gme_entity_t entity, float ax, float ay, float speed, float friction);
bool eir_gme_set_entity_aabb(eir_gme_world_t * world, eir_gme_entity_t entity, float x, float y, float width, float height);
bool eir_gme_set_world_entity_camera(eir_gme_world_t * world, eir_gme_entity_t entity, float win_scale);
bool eir_gme_set_world_entity_active_camera(eir_gme_world_t * world, eir_gme_entity_t entity);
bool eir_gme_set_world_entity_physic(eir_gme_world_t * world, eir_gme_entity_t entity, float weight);
