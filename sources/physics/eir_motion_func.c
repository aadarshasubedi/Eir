#include "eir_motion_func.h"
#include "../maths/eir_mth_func.h"

static void eir_phy_proceed_euler_integration(
   const eir_mth_vec2_t * old_position,
   eir_mth_vec2_t * new_position,
   eir_phy_motion_param_t * motion_param,
   float dtime
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
            eir_mth_vec2_t new_position;

	    eir_phy_proceed_euler_integration(
	       position,
               &new_position,
	       motion_param,
	       dtime
	       );

            position->x = new_position.x;
            position->y = new_position.y;
	 }
      }
      for (int index = 0; index < world->entities.used; ++index)
      {
	 if (world->entities.data[index] & eir_gme_component_type_aabb)
	 {
	    eir_gme_aabb_component_t * aabb = &world->aabbs.data[index];
	    eir_gme_position_component_t * position = &world->positions.data[index];

	    float x_offset = position->initial.x - aabb->x_offset;
	    float y_offset = position->initial.y - aabb->y_offset;

	    aabb->rect->position.x = position->current->x + x_offset;
	    aabb->rect->position.y = position->current->y + y_offset;
	 }
      }

      // PROCESS AABB INTERSECTION DETECTION

      for (int index = 0; index < world->entities.used; ++index)
      {
         if (world->entities.data[index] & eir_gme_component_type_aabb)
         {
            eir_gme_aabb_component_t * aabb_comp_1 = &world->aabbs.data[index];
            eir_phy_aabb_t aabb_1;

            aabb_1.position.x = aabb_comp_1->rect->position.x;
            aabb_1.position.y = aabb_comp_1->rect->position.y;
            aabb_1.size.x = aabb_comp_1->rect->size.x;
            aabb_1.size.y = aabb_comp_1->rect->size.y;

            for (int index2 = index + 1; index2 < world->entities.used; ++index2)
            {
               if (world->entities.data[index] & eir_gme_component_type_aabb)
               {
                  eir_gme_aabb_component_t * aabb_comp_2 = &world->aabbs.data[index2];
                  eir_phy_aabb_t aabb_2;
                  
                  aabb_2.position.x = aabb_comp_2->rect->position.x;
                  aabb_2.position.y = aabb_comp_2->rect->position.y;
                  aabb_2.size.x = aabb_comp_2->rect->size.x;
                  aabb_2.size.y = aabb_comp_2->rect->size.y;

                  if (eir_phy_check_aabb_intersection(&aabb_1, &aabb_2))
                  {
                     //EIR_KER_LOG_MESSAGE("aabb intersection found between entities %d and %d", index, index2);
                  }
               }
            }
         }
      }
   }
}
