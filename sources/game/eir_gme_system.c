#include "eir_gme_system.h"
#include "../fsm/eir_fsm_func.h"
#include "../physics/eir_phy_motion_func.h"

static void eir_gme_update_fsm_component_component(eir_gme_fsm_component_t * fsm_component)
{
	if (fsm_component && fsm_component->fsm)
	{
		eir_fsm_update_state_machine(fsm_component->fsm);
	}
}

static void eir_gme_update_motion_param_component(eir_gme_motion_param_component_t * motion_param_component, double dtime)
{
	// TODO
	if (motion_param_component)
	{
		eir_phy_proceed_euler_integration(
			const eir_mth_vec2_t * old_position,
			eir_mth_vec2_t * new_position,
			&motion_param_component->motion_param,
			double dtime
			);
	}
}

static void eir_gme_update_aabb_component(eir_gme_aabb_component_t * aabb, const eir_gme_position_component_t * position)
{
	if (aabb && position)
	{
		float x_offset = position->initial.x - aabb->x_offset;
		float y_offset = position->initial.y - aabb->y_offset;

		aabb->rect->position.x = position->current->x + x_offset;
		aabb->rect->position.y = position->current->y + y_offset;
	}
}

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
		direction_component->value = direction != EIR_GME_DIRECTION_UNKNOWN ? direction : component->value;
	}
}

static void eir_gme_update_camera_component(eir_gme_camera_component_t * cam)
{
   if (cam && cam->target && cam->win_rect && cam->target->rect)
   {
      // UPDATE CAM AABB

      eir_mth_vec2_t * cam_pos = &cam->win_rect->position;
      eir_mth_vec2_t * cam_size = &cam->win_rect->size;
      eir_mth_vec2_t * target_pos = &cam->target->rect->position;
      eir_mth_vec2_t * target_size = &cam->target->rect->size;
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
		for (int index; index < world->entities_flags.used; ++index)
		{
			eir_gme_entity_flags_t entity_flags = world->entities_flags.data[index];

			if (entity_flags & eir_gme_component_type_fsm)
			{
				eir_gme_fsm_component_t * fsm_component = &world->fsms.data[index];
				eir_gme_update_fsm_component_component(fsm_component);
			}
			if (entity_flags & eir_gme_component_type_motion_param)
			{
				eir_gme_motion_param_component_t * motion_param_component = &world->motion_params.data[index];
				eir_gme_update_motion_param_component(motion_param_component);
			}
			if (
				(entity_flags & eir_gme_component_type_direction)
				&& (entity_flags & eir_gme_component_type_motion_param)
				)
			{
				eir_gme_direction_component_t * direction_component = &world->directions.data[index];
				eir_gme_motion_param_component_t * motion_param_component = &world->motion_params.data[index];
				eir_gme_update_direction_component(direction_component, motion_param_component->motion_param.velocity.x, motion_param_component->motion_param.velocity.y);
			}
		}

		// ALWAYS FINISH WITH CAMERA UPDATE !
		eir_gme_update_camera_component(world->curr_camera)
	}
}