#include "eir_aabb_system.h"
#include "../physics/eir_phy_func.h"

void eir_gme_proceed_aabb_system_update(eir_gme_world_t * world)
{
   if (world)
   {
      // TODO: check entity for collision
      // entity must have a physic and aabb component
      // weight = 1.0f => cannot be moved
      // weight = 0.0f => no collision
      // weight > 0.0f && weight < 1.0f : use weight diff to minimize heaviest entity move and stop / move backward lightest entity

      // PROCESS AABB MOVE FOLLOWING ENTITIES NEW POSITION

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

      // PROCESS AABB COLLISION RESPONSE
   }
}
