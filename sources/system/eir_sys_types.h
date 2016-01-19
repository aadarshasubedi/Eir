#pragma once

#ifdef EIR_USE_SDL
#include <SDL2/SDL.h>
typedef SDL_Joystick * eir_sys_pad_handle_t;
#define EIR_SYS_INVALID_PAD_HANDLE 0
#endif

#ifdef EIR_USE_SDL
typedef SDL_Window * eir_sys_window_handle_t;
typedef SDL_GLContext eir_sys_context_handle_t;
#endif
