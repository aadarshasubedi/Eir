#pragma once

#ifdef EIR_USE_SDL
#include <SDL2/SDL.h>
typedef SDL_Joystick * eir_sys_pad_handle_t;
#define EIR_SYS_INVALID_PAD_HANDLE 0
#endif
