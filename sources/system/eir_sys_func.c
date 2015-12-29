#include "eir_sys_func.h"
#include "eir_joystick_func.h"

void eir_sys_init_env(eir_sys_env_t * sys_env)
{
   EIR_KER_LOG_MESSAGE("init system env");
   if (sys_env)
   {
      if (eir_sys_get_pad_count() > 0)
      {
	 sys_env->pad_handle = eir_sys_init_pad(0);
      }
      else
      {
	 sys_env->pad_handle = EIR_SYS_INVALID_PAD_HANDLE;
      }
   }
}

void eir_sys_release_env(eir_sys_env_t * sys_env)
{
   EIR_KER_LOG_MESSAGE("release system env");
   if (sys_env)
   {
      eir_sys_release_pad(sys_env->pad_handle);
   }
}
