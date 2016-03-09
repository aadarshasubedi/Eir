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

// TODO: remove this after ECS refacto and successful run
void eir_phy_proceed_motion_entity_update(eir_gme_world_t * world, double dtime)
{
	if (world)
	{
		eir_gme_entity_t * entity = 0;

		for (int index = 0; index < world->entities_flags.used; ++index)
		{
			if (world->entities_flags.data[index] & eir_gme_component_type_motion_param)
			{
				eir_gme_motion_param_component_t * motion_param = &world->motion_params.data[index];
				eir_mth_vec2_t * position = world->positions.data[index].current;
				eir_mth_vec2_t new_position;

				if (world->entities_flags.data[index] & eir_gme_component_type_direction)
				{
					eir_gme_direction_component_t * component = &world->directions.data[index];
					eir_gme_direction_t direction = eir_phy_get_direction_from_velocities(motion_param->data.velocity.x, motion_param->data.velocity.y);


					component->value = direction != EIR_GME_DIRECTION_UNKNOWN ? direction : component->value;
				}

				eir_phy_proceed_euler_integration(
					position,
					&new_position,
					&motion_param->data,
					dtime
					);

				position->x = new_position.x;
				position->y = new_position.y;
				if (world->entities_flags.data[index] & eir_gme_component_type_aabb)
				{
					eir_gme_aabb_component_t * aabb_comp_1 = &world->aabbs.data[index];
					eir_gme_position_component_t * position_comp_1 = &world->positions.data[index];

					for (int index2 = 0; index2 < world->entities_flags.used; ++index2)
					{
						eir_phy_update_aabb_component(aabb_comp_1, position_comp_1);

						eir_phy_aabb_t aabb_1;

						aabb_1.position.x = aabb_comp_1->rect->position.x;
						aabb_1.position.y = aabb_comp_1->rect->position.y;
						aabb_1.size.x = aabb_comp_1->rect->size.x;
						aabb_1.size.y = aabb_comp_1->rect->size.y;

						if (
							index != index2
							&& world->entities_flags.data[index2] & eir_gme_component_type_aabb
							)
						{
							eir_gme_aabb_component_t * aabb_comp_2 = &world->aabbs.data[index2];
							eir_phy_aabb_t aabb_2;

							aabb_2.position.x = aabb_comp_2->rect->position.x;
							aabb_2.position.y = aabb_comp_2->rect->position.y;
							aabb_2.size.x = aabb_comp_2->rect->size.x;
							aabb_2.size.y = aabb_comp_2->rect->size.y;

							if (eir_phy_check_aabb_intersection(&aabb_1, &aabb_2))
							{
								if (
									world->entities_flags.data[index2] & eir_gme_component_type_physic
									&& world->entities_flags.data[index] & eir_gme_component_type_physic
									&& world->physics.data[index2].weight > 0.0f
									&& world->physics.data[index].weight > 0.0f
									)
								{
									float x_depth = eir_phy_get_x_aabb_intersection_depth(&aabb_1, &aabb_2);
									float y_depth = eir_phy_get_y_aabb_intersection_depth(&aabb_1, &aabb_2);

									if (eir_mth_abs(x_depth) < eir_mth_abs(y_depth))
									{
										position->x += x_depth;
									}
									else
									{
										position->y += y_depth;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}