#pragma once

#include "eir_gfx_types.h"
#include "eir_gfx_sprite_batch.h"
#include "eir_gfx_rect_batch.h"
#include "eir_gfx_image.h"
#include "eir_gfx_texture.h"
#include "../maths/eir_mth_matrix.h"
#include "eir_gfx_viewport.h"
#include "eir_gfx_group.h"


typedef struct
{
   eir_gfx_image_array_t images;
   eir_gfx_texture_array_t textures;
   
   eir_gfx_api_shader_handle_t text_vert_shader;
   eir_gfx_api_shader_handle_t text_geom_shader;
   eir_gfx_api_shader_handle_t text_frag_shader;
   eir_gfx_api_program_handle_t text_program;
   eir_gfx_image_t text_image;
   eir_gfx_texture_t text_texture;

   eir_gfx_api_shader_handle_t sprite_vert_shader;
   eir_gfx_api_shader_handle_t sprite_geom_shader;
   eir_gfx_api_shader_handle_t sprite_frag_shader;
   eir_gfx_api_program_handle_t sprite_program;

   eir_gfx_api_shader_handle_t rect_vert_shader;
   eir_gfx_api_shader_handle_t rect_geom_shader;
   eir_gfx_api_shader_handle_t rect_frag_shader;
   eir_gfx_api_program_handle_t rect_program;

	eir_gfx_group_array_t groups;
   eir_gfx_group_t ** sorted_groups;

   eir_gfx_viewport_t viewport;
   eir_mth_mat4_t projection;
   eir_mth_mat4_t view;
} eir_gfx_env_t;
