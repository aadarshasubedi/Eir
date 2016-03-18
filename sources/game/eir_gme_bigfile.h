#pragma once

#include "../kernel/eir_ker_array_macro.h"
#include "eir_gme_world.h"

#define EIR_GME_BIGFILE_PATH_MAX_BUFFER 1024
#define EIR_GME_BIGFILE_PATH_MAX_BYTES_READ EIR_GME_BIGFILE_PATH_MAX_BUFFER - 1

typedef struct
{
	char file_path[EIR_GME_BIGFILE_PATH_MAX_BUFFER];
	int invert_image;
} eir_gme_image_info_t;

typedef struct
{
	int image_index;
} eir_gme_texture_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_image_info_t,
	eir_gme_image_info_array_t
	);

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_texture_info_t,
	eir_gme_texture_info_array_t
	);

#define EIR_GME_PARSE_image(buffer, item, bytes_read) \
   sscanf(                                            \
      buffer,                                         \
      "%1023[^|\n]|%d|%n",                            \
      item.file_path,                                 \
      &item.invert_image,                             \
      &bytes_read                                     \
      );

#define EIR_GME_PARSE_texture(buffer, item, bytes_read)  \
   sscanf(                                               \
      buffer,                                            \
      "%d|%n",                                           \
      &item.image_index,                                 \
      &bytes_read                                        \
      );

typedef struct
{
	int file_version;
	int image_count;
	int texture_count;
	int entity_count;
	int aabb_count;
	int based_melee_attack_count;
	int color_count;
	int direction_count;
	int motion_param_count;
	int physic_count;
	int position_count;
	int size_count;
	int sprite_count;
	int state_count;
} eir_gme_bigfile_header_t;

typedef struct
{
	eir_gme_bigfile_header_t header;
	eir_gme_image_info_array_t images;
	eir_gme_texture_info_array_t textures;
	eir_gme_aabb_info_array_t aabbs;
	eir_gme_based_melee_attack_info_array_t based_melee_attacks;
	eir_gme_color_info_array_t colors;
	eir_gme_direction_info_array_t directions;
	eir_gme_motion_param_info_array_t motion_params;
	eir_gme_physic_info_array_t physics;
	eir_gme_position_info_array_t positions;
	eir_gme_size_info_array_t sizes;
	eir_gme_sprite_info_array_t sprites;
	eir_gme_state_info_array_t states;
} eir_gme_bigfile_t;
