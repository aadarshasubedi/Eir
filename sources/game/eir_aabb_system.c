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



      // PROCESS AABB COLLISION RESPONSE
   }
}
