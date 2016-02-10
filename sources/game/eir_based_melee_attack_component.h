#pragma once

#include "../physics/eir_aabb.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
	eir_phy_aabb_t damage_zone;
	float damage;
	bool active;
} eir_gme_based_melee_attack_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_based_melee_attack_component_t, eir_gme_based_melee_attack_component_array_t);
