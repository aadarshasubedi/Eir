#pragma once

#include "eir_gfx_types.h"
#include "eir_sprite_batch.h"
#include "eir_vertex_batch.h"
#include "eir_rect_batch.h"
#include "eir_image.h"
#include "eir_texture.h"
#include "eir_sprite_ref.h"
#include "../kernel/eir_array_macro.h" // TODO: remove this include
#include "../maths/eir_matrix.h"
#include "eir_viewport.h"

// TODO: move array definition in dedicated struct definition include file
EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_batch_t, eir_gfx_sprite_batch_array_t);
EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_image_t, eir_gfx_image_array_t);
EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_sprite_ref_t, eir_gfx_sprite_ref_array_t);
EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_texture_t, eir_gfx_texture_array_t);

typedef struct
{
   eir_gfx_window_handle_t window;
   eir_gfx_context_handle_t context;
   eir_gfx_sprite_batch_array_t sprite_batches;
   eir_gfx_image_array_t images;
   eir_gfx_sprite_ref_array_t sprites_ref;
   eir_gfx_texture_array_t textures;
   
   eir_gfx_sprite_batch_array_t text_batches;
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
   
   eir_gfx_vertex_batch_t line_batch;
   eir_gfx_vertex_batch_t quad_batch; // TODO: rename it
   eir_gfx_rect_batch_t rect_batch;
   eir_gfx_api_shader_handle_t default_vert_shader;
   eir_gfx_api_shader_handle_t default_frag_shader;
   eir_gfx_api_program_handle_t default_program;

   eir_gfx_viewport_t viewport;
   eir_mth_mat4_t projection;
   eir_mth_mat4_t view;
} eir_gfx_env_t;
