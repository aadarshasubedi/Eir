#include "eir_camera_system.h"

void eir_gme_proceed_camera_system_update(eir_gme_world_t * world)
{
   eir_gme_camera_component_t * cam = 0;

   if (world)
   {
      cam = world->curr_camera;
   }
   if (cam && cam->target)
   {
      /* TODO:
       * - check if
       */
      
   }
}
