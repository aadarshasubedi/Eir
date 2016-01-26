#include "eir_camera_system.h"
#include "../maths/eir_mth_func.h"

void eir_gme_proceed_camera_system_update(eir_gme_world_t * world)
{
   eir_gme_camera_component_t * cam = 0;

   if (world)
   {
      cam = world->curr_camera;
   }
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

      cam->position.x -= target_pos->x - cam->prev_position.x;
      cam->position.y -= target_pos->y - cam->prev_position.y;
      cam->prev_position.x = target_pos->x;
      cam->prev_position.y = target_pos->y;
   }
}
