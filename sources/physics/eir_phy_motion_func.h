#pragma once

#include "../game/eir_gme_world.h"

void eir_phy_proceed_euler_integration(
	const eir_mth_vec2_t * old_position,
	eir_mth_vec2_t * new_position,
	eir_phy_motion_param_t * motion_param,
	double dtime
	);
//void eir_phy_proceed_motion_entity_update(eir_gme_world_t * world, double dtime);
