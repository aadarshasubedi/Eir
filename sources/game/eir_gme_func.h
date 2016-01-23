#pragma once

#include "eir_gme_env.h"

void eir_gme_init_env(eir_gme_env_t * env);
void eir_gme_world_entity_update_linked_components(eir_gme_world_t * world);
void eir_gme_release_env(eir_gme_env_t * env);
