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
#define EIR_GFX_INVALID_TEXTURE_HANDLE 0
#define EIR_GFX_INVALID_VAO_HANDLE 0
#define EIR_GFX_INVALID_VBO_HANDLE 0
#define EIR_GFX_INVALID_PROGRAM_HANDLE 0
#endif

#ifdef EIR_USE_SDL
typedef SDL_Window * eir_gfx_window_handle_t;
typedef SDL_GLContext eir_gfx_context_handle_t;
#endif

typedef enum
{
   eir_gfx_primitive_type_unknown = 0,
   eir_gfx_primitive_type_lines = 1,
   eir_gfx_primitive_type_quads = 2
} eir_gfx_primitive_type_t;
