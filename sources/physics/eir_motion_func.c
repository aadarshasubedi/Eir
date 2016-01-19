#include "eir_motion_func.h"
#include "../maths/eir_mth_func.h"

static void eir_phy_proceed_euler_integration(
   eir_mth_vec2_t * position,
   eir_phy_motion_param_t * motion_param,
   float dtime
   )
{
   eir_mth_vec2_t acceleration;

   if (motion_param && position)
   {
      acceleration.x = motion_param->acceleration.x * motion_param->speed_factor;
      acceleration.y = motion_param->acceleration.y * motion_param->speed_factor;
      acceleration.x += -motion_param->friction_factor * motion_param->velocity.x;
      acceleration.y += -motion_param->friction_factor * motion_param->velocity.y;
      position->x += 0.5f * acceleration.x * eir_mth_square_f(dtime) +
	 motion_param->velocity.x * dtime;
      position->y += 0.5f * acceleration.y * eir_mth_square_f(dtime) +
	 motion_param->velocity.y * dtime;
      motion_param->velocity.x += acceleration.x * dtime;
      motion_param->velocity.y += acceleration.y * dtime;
   }
}

void eir_phy_proceed_motion_entity_update(eir_gme_world_t * world, float dtime)
{
   if (world)
   {
      eir_gme_entity_t * entity = 0;

      for (int index = 0; index < world->entities.used; ++index)
      {
	 if (world->entities.data[index] & eir_gme_component_type_motion_param)
	 {
	    eir_phy_motion_param_t * motion_param = &world->motion_params.data[index];
	    eir_mth_vec2_t * position = world->positions.data[index].current;

	    eir_phy_proceed_euler_integration(
	       position,
	       motion_param,
	       dtime
	       );
	    if (world->entities.data[index] & eir_gme_component_type_aabb)
	    {
	       eir_gme_aabb_component_t * aabb = &world->aabbs.data[index];

	       if (aabb && aabb->curr_rect)
	       {
		  //aabb->curr_rect->position.x = aabb->aabb.position.x + position->x;
		  //aabb->curr_rect->position.y = aabb->aabb.position.y + position->y;
	       }
	    }
	 }
      }
   }
}
