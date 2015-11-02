#pragma once

#ifdef EIR_USE_OPENGL
#ifdef EIR_USE_SDL
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#endif
#endif

#ifdef EIR_USE_OPENGL
typedef unsigned int eir_gfx_api_vbo_handle_t;
typedef unsigned int eir_gfx_api_vao_handle_t;
typedef unsigned int eir_gfx_api_texture_handle_t;
typedef unsigned int eir_gfx_api_program_handle_t;
typedef unsigned int eir_gfx_api_att_handle_t;
typedef unsigned int eir_gfx_api_uni_handle_t;
typedef unsigned int eir_gfx_api_shader_handle_t;
#endif

#ifdef EIR_USE_SDL
typedef SDL_Window * eir_gfx_window_handle_t;
typedef SDL_GLContext eir_gfx_context_handle_t;
#endif
