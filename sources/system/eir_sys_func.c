#include "eir_sys_func.h"
#include "eir_sys_joystick_func.h"

void eir_sys_init_env(eir_sys_env_t * sys_env)
{
   EIR_KER_LOG_MESSAGE("init system env");
   if (sys_env)
   {
      for (int index = 0; index < EIR_GME_TOTAL_INPUT_CONTROLLER - 1; ++index)
      {
	 if (index < eir_sys_get_pad_count())
	 {
	    sys_env->pad_handles[index] = eir_sys_init_pad(index);
	 }
	 else
	 {
	    sys_env->pad_handles[index] = EIR_SYS_INVALID_PAD_HANDLE;
	 }
      }
   }
}

void eir_sys_release_env(eir_sys_env_t * sys_env)
{
   EIR_KER_LOG_MESSAGE("release system env");
   if (sys_env)
   {
      for (int index = 0; index < EIR_GME_TOTAL_INPUT_CONTROLLER - 1; ++index)
      {
	 eir_sys_release_pad(sys_env->pad_handles[index]);
      }
   }
}
