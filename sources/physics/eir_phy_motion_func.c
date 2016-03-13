#include "eir_phy_motion_func.h"
#include "eir_phy_func.h"
#include "../maths/eir_mth_func.h"

void eir_phy_proceed_euler_integration(
	const eir_mth_vec2_t * old_position,
	eir_mth_vec2_t * new_position,
	eir_phy_motion_param_t * motion_param,
	double dtime
	)
{
	eir_mth_vec2_t acceleration;

	if (motion_param && old_position && new_position)
	{
		acceleration.x = motion_param->acceleration.x * motion_param->speed_factor;
		acceleration.y = motion_param->acceleration.y * motion_param->speed_factor;
		acceleration.x += -motion_param->friction_factor * motion_param->velocity.x;
		acceleration.y += -motion_param->friction_factor * motion_param->velocity.y;
		new_position->x = old_position->x + 0.5f * acceleration.x * eir_mth_square_f(dtime) + motion_param->velocity.x * dtime;
		new_position->y = old_position->y + 0.5f * acceleration.y * eir_mth_square_f(dtime) + motion_param->velocity.y * dtime;
		motion_param->velocity.x += acceleration.x * dtime;
		motion_param->velocity.y += acceleration.y * dtime;
	}
}