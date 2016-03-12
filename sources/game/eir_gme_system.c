#include "eir_gme_system.h"
#include "../fsm/eir_fsm_func.h"
#include "../physics/eir_phy_motion_func.h"
#include "../graphics/eir_gfx_func.h"

static void eir_gme_update_direction_component(eir_gme_direction_component_t * direction_component, float x_velocity, float y_velocity)
{
	if (direction_component)
	{
		eir_gme_direction_t direction = EIR_GME_DIRECTION_UNKNOWN;
		eir_gme_direction_t x_direction = EIR_GME_DIRECTION_UNKNOWN;
		eir_gme_direction_t y_direction = EIR_GME_DIRECTION_UNKNOWN;

		if (eir_mth_abs(x_velocity) > EIR_GME_DIRECTION_EPSILON_VALUE)
		{
			x_direction = x_velocity > 0.0f ? EIR_GME_DIRECTION_RIGHT : EIR_GME_DIRECTION_LEFT;
		}
		if (eir_mth_abs(y_velocity) > EIR_GME_DIRECTION_EPSILON_VALUE)
		{
			y_direction = y_velocity > 0.0f ? EIR_GME_DIRECTION_BOTTOM : EIR_GME_DIRECTION_UP;
		}
		if (x_direction != EIR_GME_DIRECTION_UNKNOWN && y_direction != EIR_GME_DIRECTION_UNKNOWN)
		{
			if (x_direction == EIR_GME_DIRECTION_RIGHT && y_direction == EIR_GME_DIRECTION_UP)
			{
				direction = EIR_GME_DIRECTION_UP_RIGHT;
			}
			if (x_direction == EIR_GME_DIRECTION_RIGHT && y_direction == EIR_GME_DIRECTION_BOTTOM)
			{
				direction = EIR_GME_DIRECTION_BOTTOM_RIGHT;
			}
			if (x_direction == EIR_GME_DIRECTION_LEFT && y_direction == EIR_GME_DIRECTION_UP)
			{
				direction = EIR_GME_DIRECTION_UP_LEFT;
			}
			if (x_direction == EIR_GME_DIRECTION_LEFT && y_direction == EIR_GME_DIRECTION_BOTTOM)
			{
				direction = EIR_GME_DIRECTION_BOTTOM_LEFT;
			}
		}
		else if (x_direction != EIR_GME_DIRECTION_UNKNOWN)
		{
			direction = x_direction;
		}
		else if (y_direction != EIR_GME_DIRECTION_UNKNOWN)
		{
			direction = y_direction;
		}
		direction_component->direction = direction != EIR_GME_DIRECTION_UNKNOWN ? direction : direction_component->direction;
	}
}

static void eir_gme_update_camera(eir_gme_camera_t * cam)
{
   if (cam && cam->target)
   {
      // UPDATE CAM AABB

      eir_mth_vec2_t * cam_pos = &cam->win_aabb.position;
      eir_mth_vec2_t * cam_size = &cam->win_aabb.size;
      eir_mth_vec2_t * target_pos = &cam->target->aabb.position;
      eir_mth_vec2_t * target_size = &cam->target->aabb.size;
      float x = 0.0f;
      float y = 0.0f;

      if (target_pos->x < cam_pos->x)
      {
         x = -(eir_mth_max(eir_mth_abs(cam_pos->x), eir_mth_abs(target_pos->x)) - eir_mth_min(eir_mth_abs(cam_pos->x), eir_mth_abs(target_pos->x)));
      }
      if ((target_pos->x + target_size->x) > (cam_pos->x + cam_size->x))
      {
         x = eir_mth_max(eir_mth_abs(cam_pos->x + cam_size->x), eir_mth_abs(target_pos->x + target_size->x)) - eir_mth_min(eir_mth_abs(cam_pos->x + cam_size->x), eir_mth_abs(target_pos->x + target_size->x));
      }
      if (target_pos->y < cam_pos->y)
      {
         y = -(eir_mth_max(eir_mth_abs(cam_pos->y), eir_mth_abs(target_pos->y)) - eir_mth_min(eir_mth_abs(cam_pos->y), eir_mth_abs(target_pos->y)));
      }
      if ((target_pos->y + target_size->y) > (cam_pos->y + cam_size->y))
      {
         y = eir_mth_max(eir_mth_abs(cam_pos->y + cam_size->y), eir_mth_abs(target_pos->y + target_size->y)) - eir_mth_min(eir_mth_abs(cam_pos->y + cam_size->y), eir_mth_abs(target_pos->y + target_size->y));
      }
      cam_pos->x += x;
      cam_pos->y += y;

      // UPDATE CAM POSITION

      cam->position.x -= cam_pos->x - cam->prev_position.x;
      cam->position.y -= cam_pos->y - cam->prev_position.y;
      cam->prev_position.x = cam_pos->x;
      cam->prev_position.y = cam_pos->y;
   }
}

void eir_gme_update_all_components_systems(eir_gme_world_t * world, double dtime)
{
	if (world)
	{
		for (int index = 0; index < world->entities_flags.used; ++index)
		{
			eir_gme_entity_flags_t entity_flags = world->entities_flags.data[index];

			if (entity_flags & eir_gme_component_type_fsm)
			{
				eir_gme_fsm_component_t * fsm_component = &world->fsms.data[index];
				eir_fsm_update_state_machine(fsm_component->fsm);
			}
			if (
				(entity_flags & eir_gme_component_type_motion_param)
				&& (entity_flags & eir_gme_component_type_position)
				)
			{
				eir_gme_motion_param_component_t * motion_param_component = &world->motion_params.data[index];
				eir_gme_position_component_t * position_component = &world->positions.data[index];
				eir_mth_vec2_t new_position = { .x = 0.0f, .y = 0.0f };

				eir_phy_proceed_euler_integration(
					&position_component->position,
					&new_position,
					&motion_param_component->motion_param,
					dtime
					);
				eir_gme_set_entity_position(
					world,
					index,
					new_position.x,
					new_position.y
					);
			}
			if (entity_flags & eir_gme_component_type_aabb)
			{
				eir_gme_aabb_component_t * aabb_component = &world->aabbs.data[index];
				eir_gme_position_component_t * position_component = 0;

				if (entity_flags & eir_gme_component_type_position)
				{
					position_component = &world->positions.data[index];
					if (position_component->modified)
					{
						aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
					}
				}
				if (entity_flags & eir_gme_component_type_physic)
				{
					for (int index2 = 0; index2 < world->entities_flags.used; ++index2)
					{
						if (
							(index != index2)
							&& (world->entities_flags.data[index2] & eir_gme_component_type_aabb)
							&& (world->entities_flags.data[index2] & eir_gme_component_type_physic)
							)
						{
							eir_gme_aabb_component_t * aabb_component_2 = &world->aabbs.data[index];
							bool collision = eir_phy_check_aabb_intersection(
								&world->aabbs.data[index].aabb,
								&world->aabbs.data[index2].aabb
								);

							if (collision)
							{
								if (
									world->physics.data[index].weight > 0.0f
									&& world->physics.data[index2].weight > 0.0f
									&& position_component
									)
								{
									float x_depth = eir_phy_get_x_aabb_intersection_depth(
										&world->aabbs.data[index].aabb,
										&world->aabbs.data[index2].aabb
										);
									float y_depth = eir_phy_get_y_aabb_intersection_depth(
										&world->aabbs.data[index].aabb,
										&world->aabbs.data[index2].aabb
										);

									float x = position_component->position.x;
									float y = position_component->position.y;

									if (eir_mth_abs(x_depth) < eir_mth_abs(y_depth))
									{
										x += x_depth;
									}
									else
									{
										y += y_depth;
									}
									eir_gme_set_entity_position(
										world,
										index,
										x,
										y
										);
								}
							}
						}
					}
				}
			}
			if (
				(entity_flags & eir_gme_component_type_direction)
				&& (entity_flags & eir_gme_component_type_motion_param)
				)
			{
				eir_gme_direction_component_t * direction_component = &world->directions.data[index];
				eir_gme_motion_param_component_t * motion_param_component = &world->motion_params.data[index];
				eir_gme_update_direction_component(
					direction_component,
					motion_param_component->motion_param.velocity.x,
					motion_param_component->motion_param.velocity.y
					);
			}
			if (entity_flags & eir_gme_component_type_sprite)
			{
				eir_gme_sprite_component_t * sprite_component = &world->sprites.data[index];
				eir_gfx_sprite_proxy_t * sprite_proxy = sprite_component->sprite_proxy;


				eir_mth_vec2_t position =
				{
					.x = sprite_proxy->position.x,
					.y = sprite_proxy->position.y
				};
				eir_mth_vec2_t size =
				{
					.x = sprite_proxy->size.x,
					.y = sprite_proxy->size.y
				};
				eir_mth_vec2_t uv_offset =
				{
					.x = sprite_proxy->uv_offset.x,
					.y = sprite_proxy->uv_offset.y
				};
				eir_mth_vec2_t uv_size =
				{
					.x = sprite_proxy->uv_size.x,
					.y = sprite_proxy->uv_size.y
				};
				eir_gfx_color_t color =
				{
					.r = sprite_proxy->color.r,
					.g = sprite_proxy->color.g,
					.b = sprite_proxy->color.b,
					.a = sprite_proxy->color.a
				};

				position.x = sprite_proxy->position.x;
				position.x = sprite_proxy->position.y;

				bool must_modify_sprite = false;

				if (entity_flags & eir_gme_component_type_position)
				{
					eir_gme_position_component_t * position_component = &world->positions.data[index];

					if (position_component->modified)
					{
						position.x = position_component->position.x;
						position.y = position_component->position.y;
						position_component->modified = false;
						must_modify_sprite = true;
					}
				}
				if (entity_flags & eir_gme_component_type_size)
				{
					eir_gme_size_component_t * size_component = &world->sizes.data[index];

					if (size_component->modified)
					{
						size.x = size_component->size.x;
						size.y = size_component->size.y;
						must_modify_sprite = true;
					}
				}
				if (entity_flags & eir_gme_component_type_color)
				{
					eir_gme_color_component_t * color_component = &world->colors.data[index];

					if (color_component->modified)
					{
						color.r = color_component->color.r;
						color.g = color_component->color.g;
						color.b = color_component->color.b;
						color.a = color_component->color.a;
						must_modify_sprite = true;
					}
				}
				if (must_modify_sprite)
				{
					eir_gfx_modify_sprite(
	   					sprite_proxy,
	   					&position,
	   					&size,
	   					&uv_offset,
	   					&uv_size,
	   					&color
	   					);
				}
			}

			// CLEAM MODIFIED FLAGS
			
			if (entity_flags & eir_gme_component_type_position)
			{
				world->positions.data[index].modified = false;
			}
			if (entity_flags & eir_gme_component_type_size)
			{
				world->sizes.data[index].modified = false;
			}
			if (entity_flags & eir_gme_component_type_color)
			{
				world->colors.data[index].modified = false;
			}
		}

		// ALWAYS FINISH WITH CAMERA UPDATE !

		//eir_gme_update_camera(&world->camera);
	}
}