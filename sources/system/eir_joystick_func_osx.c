#include <SDL2/SDL.h>
#include "eir_joystick_func.h"

int eir_sys_get_joystick_count()
{
   return SDL_NumJoysticks();
}

eir_sys_joystick_handle_t eir_sys_get_joystick(int joystick_index)
{
   return SDL_JoystickOpen(joystick_index);
}

void eir_sys_close_joystick(eir_sys_joystick_handle_t handle)
{
   if (handle && 0 < SDL_NumJoysticks())
   {
      SDL_JoystickClose(handle);
   }
}
