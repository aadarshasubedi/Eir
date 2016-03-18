#pragma once

#include "../physics/eir_phy_aabb.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_phy_aabb_t damage_zone;
	float damage;
	bool active;
} eir_gme_based_melee_attack_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_based_melee_attack_component_t,
	eir_gme_based_melee_attack_component_array_t
	);

typedef struct
{
	int entity_index;
	float damage;
	float damage_zone_x;
	float damage_zone_y;
	float damage_zone_width;
	float damage_zone_height;
} eir_gme_based_melee_attack_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_based_melee_attack_info_t,
	eir_gme_based_melee_attack_info_array_t
	);

#define EIR_GME_PARSE_based_melee_attack(buffer, item, bytes_read)  \
   sscanf(                                            \
      buffer,                                         \
      "%d|%f|%f|%f|%f|%f|%n",                         \
      &item.entity_index,                             \
      &item.damage,                                   \
      &item.damage_zone_x,                            \
      &item.damage_zone_y,                            \
      &item.damage_zone_width,                        \
      &item.damage_zone_height,                       \
      &bytes_read                                     \
      );
