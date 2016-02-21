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

typedef enum
{
   eir_gfx_item_flag_default           = 0,
   eir_gfx_item_flag_editable          = 1 << 0,
   eir_gfx_item_flag_use_screen_coord  = 1 << 1,
   eir_gfx_item_flag_built             = 1 << 2,
   eir_gfx_item_flag_visible           = 1 << 3,
   eir_gfx_item_flag_added             = 1 << 4,
   eir_gfx_item_flag_modified          = 1 << 5
} eir_gfx_item_info_flag_t;

typedef int eir_gfx_item_info_t;
