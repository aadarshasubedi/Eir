#include "eir_motion_func.h"

void eir_phy_proceed_euler_integration(
   const eir_mth_vec2_t * old_position,
   const eir_phy_motion_param_t * motion_param,
   double elapsed_time,
   eir_mth_vec2_t * out_new_position
   )
{
   if (!old_position || !motion_param || !out_new_position)
   {
      return;
   }
   out_new_position->x = old_position->x + motion_param->velocity.x * elapsed_time;
   out_new_position->y = old_position->y + motion_param->velocity.y * elapsed_time;
   motion_param->velocity.x += motion_param->acceleration.x * elapsed_time;
   motion_param->velocity.y += motion_param->acceleration.y * elapsed_time;
   if (motion_param->velocity.x > motion_param->max_velocity.x)
   {
      motion_param->velocity.x = motion_param->max_velocity.x;
      motion_param->velocity.y = motion_param->max_velocity.y;
   }
}
