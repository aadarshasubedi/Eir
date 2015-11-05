#include "eir_sys_func.h"

void eir_sys_init(eir_sys_env_t * sys_env)
{
   if (!sys_env)
   {
      return;
   }

   sys_env->joystick.x_axis_value = 0.0f;
   sys_env->joystick.y_axis_value = 0.0f;
}
