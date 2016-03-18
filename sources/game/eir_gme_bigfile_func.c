#include <stdio.h>
#include "eir_gme_bigfile_func.h"
#include "../system/eir_sys_file_system.h"
#include "../kernel/eir_ker_log.h"

// -----------------------------------------------------------------------------
static void eir_gme_remove_bigfile_separator(char ** buffer)
{
   if (buffer)
   {
      char * tmp = *buffer;

      while (tmp && (tmp[0] == '|' || tmp[0] == '\n'))
      {
         ++tmp;
      }
      *buffer = tmp;
   }
}

// -----------------------------------------------------------------------------
#define EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, item_name)       \
   if (buffer && bigfile)                                            \
   {                                                                 \
      for (int ii = 0; ii < bigfile->header.item_name##_count; ++ii) \
      {                                                              \
         int bytes_read = 0;                                         \
         EIR_GME_PARSE_##item_name(                                  \
            buffer,                                                  \
            bigfile->item_name##s.data[ii],                          \
            bytes_read                                               \
            );                                                       \
         if (bytes_read > 0)                                         \
         {                                                           \
            buffer += bytes_read;                                    \
            eir_gme_remove_bigfile_separator(&buffer);               \
         }                                                           \
         else                                                        \
         {                                                           \
            EIR_KER_LOG_ERROR(                                       \
               "CANNOT PARSE item_name: INVALID_BODY"                \
               );                                                    \
         }                                                           \
      }                                                              \
   }

// -----------------------------------------------------------------------------
static void eir_gme_init_bigfile(eir_gme_bigfile_t * bigfile)
{
   if (bigfile)
   {
      bigfile->header.file_version = 0;
      bigfile->header.image_count = 0;
      bigfile->header.texture_count = 0;
      bigfile->header.entity_count = 0;
      bigfile->header.aabb_count = 0;
      bigfile->header.based_melee_attack_count = 0;
      bigfile->header.color_count = 0;
      bigfile->header.direction_count = 0;
      bigfile->header.motion_param_count = 0;
      bigfile->header.physic_count = 0;
      bigfile->header.position_count = 0;
      bigfile->header.size_count = 0;
      bigfile->header.sprite_count = 0;
      bigfile->header.state_count = 0;
      EIR_KER_INIT_ARRAY(bigfile->images);
      EIR_KER_INIT_ARRAY(bigfile->textures);
      EIR_KER_INIT_ARRAY(bigfile->aabbs);
      EIR_KER_INIT_ARRAY(bigfile->based_melee_attacks);
      EIR_KER_INIT_ARRAY(bigfile->colors);
      EIR_KER_INIT_ARRAY(bigfile->directions);
      EIR_KER_INIT_ARRAY(bigfile->motion_params);
      EIR_KER_INIT_ARRAY(bigfile->physics);
      EIR_KER_INIT_ARRAY(bigfile->positions);
      EIR_KER_INIT_ARRAY(bigfile->sizes);
      EIR_KER_INIT_ARRAY(bigfile->sprites);
      EIR_KER_INIT_ARRAY(bigfile->states);
   }
}

// -----------------------------------------------------------------------------
static void eir_gme_parse_bigfile_header(
   char * buffer,
   eir_gme_bigfile_t * bigfile
   )
{
   if (bigfile && buffer)
   {
      int bytes_read = 0;

      sscanf(
         buffer,
         "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d%n",
         &bigfile->header.file_version,
         &bigfile->header.image_count, 
         &bigfile->header.texture_count,
         &bigfile->header.entity_count,
         &bigfile->header.aabb_count,
         &bigfile->header.based_melee_attack_count,
         &bigfile->header.color_count,
         &bigfile->header.direction_count,
         &bigfile->header.motion_param_count,
         &bigfile->header.physic_count,
         &bigfile->header.position_count,
         &bigfile->header.size_count,
         &bigfile->header.sprite_count,
         &bigfile->header.state_count,
         &bytes_read
         );
      if (bytes_read > 0)
      {
         buffer += bytes_read;
         eir_gme_remove_bigfile_separator(&buffer);
      }
      else
      {
         EIR_KER_LOG_ERROR("CANNOT READ BIG FILE HEADER: INVALID HEADER");
      }
   }
   else
   {
      EIR_KER_LOG_ERROR(
         "CANNOT READ BIG FILE HEADER: INVALID BUFFER OR BIGFILE INSTANCE"
         );
   }
}

static void eir_gme_allocate_bigfile_arrays(eir_gme_bigfile_t * bigfile)
{
   if (bigfile)
   {
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_image_info_t,
         bigfile->images,
         bigfile->header.image_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_texture_info_t,
         bigfile->textures,
         bigfile->header.texture_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_aabb_info_t,
         bigfile->aabbs,
         bigfile->header.aabb_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_based_melee_attack_info_t,
         bigfile->based_melee_attacks,
         bigfile->header.based_melee_attack_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_color_info_t,
         bigfile->colors,
         bigfile->header.color_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_direction_info_t,
         bigfile->directions,
         bigfile->header.direction_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_motion_param_info_t,
         bigfile->motion_params,
         bigfile->header.motion_param_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_physic_info_t,
         bigfile->physics,
         bigfile->header.physic_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_position_info_t,
         bigfile->positions,
         bigfile->header.position_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_size_info_t,
         bigfile->sizes,
         bigfile->header.size_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_sprite_info_t,
         bigfile->sprites,
         bigfile->header.sprite_count
         );
      EIR_KER_ALLOCATE_ARRAY(
         eir_gme_state_info_t,
         bigfile->states,
         bigfile->header.state_count
         );
   }
}

// -----------------------------------------------------------------------------
static bool eir_gme_parse_bigfile_body(
   char * buffer,
   eir_gme_bigfile_t * bigfile
   )
{
   bool result = false;

   if (bigfile && buffer)
   {
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, image);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, texture);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, aabb);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, based_melee_attack);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, color);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, direction);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, motion_param);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, physic);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, position);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, size);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, sprite);
      EIR_GME_PARSE_BIGFILE_ITEM(buffer, bigfile, state);
   }
   return result;
}

// -----------------------------------------------------------------------------
bool eir_gme_parse_bigfile(
   const char * bigfile_path,
   eir_gme_bigfile_t * bigfile
   )
{
   bool result = false;

   if (bigfile)
   {
      eir_gme_init_bigfile(bigfile);

      char * buffer = 0;
      
      result = eir_sys_read_file(bigfile_path, false, &buffer);
      if (result)
      {
         eir_gme_parse_bigfile_header(buffer, bigfile);
         eir_gme_allocate_bigfile_arrays(bigfile);
         eir_gme_parse_bigfile_body(buffer, bigfile);
      }
   }
   return result;
}

// -----------------------------------------------------------------------------
bool eir_gme_generate_all_entities(
   const eir_gme_bigfile_t * bigfile,
   eir_ker_env_t * env
   )
{
   bool result = false;

   if (bigfile)
   {
   }
   return result;
}

// -----------------------------------------------------------------------------
void eir_gme_release_bigfile(
   eir_gme_bigfile_t * bigfile
   )
{
   if (bigfile)
   {
      EIR_KER_FREE_ARRAY(bigfile->images);
      EIR_KER_FREE_ARRAY(bigfile->textures);
      EIR_KER_FREE_ARRAY(bigfile->aabbs);
      EIR_KER_FREE_ARRAY(bigfile->based_melee_attacks);
      EIR_KER_FREE_ARRAY(bigfile->colors);
      EIR_KER_FREE_ARRAY(bigfile->directions);
      EIR_KER_FREE_ARRAY(bigfile->motion_params);
      EIR_KER_FREE_ARRAY(bigfile->physics);
      EIR_KER_FREE_ARRAY(bigfile->positions);
      EIR_KER_FREE_ARRAY(bigfile->sizes);
      EIR_KER_FREE_ARRAY(bigfile->sprites);
      EIR_KER_FREE_ARRAY(bigfile->states);
      bigfile->header.file_version = 0;
      bigfile->header.image_count = 0;
      bigfile->header.texture_count = 0;
      bigfile->header.entity_count = 0;
      bigfile->header.aabb_count = 0;
      bigfile->header.based_melee_attack_count = 0;
      bigfile->header.color_count = 0;
      bigfile->header.direction_count = 0;
      bigfile->header.motion_param_count = 0;
      bigfile->header.physic_count = 0;
      bigfile->header.position_count = 0;
      bigfile->header.size_count = 0;
      bigfile->header.sprite_count = 0;
      bigfile->header.state_count = 0;
   }
}
