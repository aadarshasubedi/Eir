#include "eir_aabb_system.h"

void eir_gme_proceed_aabb_system_update(eir_gme_world_t * world)
{
   if (world)
   {
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
   }
}
