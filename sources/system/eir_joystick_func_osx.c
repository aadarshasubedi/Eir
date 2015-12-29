#include <SDL2/SDL.h>
#include "eir_joystick_func.h"

int eir_sys_get_pad_count()
{
   return SDL_NumJoysticks();
}

eir_sys_pad_handle_t eir_sys_init_pad(int pad_index)
{
   return SDL_JoystickOpen(pad_index);
}

void eir_sys_release_pad(eir_sys_pad_handle_t pad_handle)
{
   if (SDL_JoystickGetAttached(pad_handle))
   {
      SDL_JoystickClose(pad_handle);
   }
}
