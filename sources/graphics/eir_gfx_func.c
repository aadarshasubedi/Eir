#include "eir_gfx_func.h"
#include "eir_gfx_api_func.h"
#include "../maths/eir_mth_func.h"

// ---------------------------------------------------------------------------
static void eir_gfx_init_image(eir_gfx_image_t * image)
{
   if (image)
   {
      image->pixels = 0;
      image->width = 0;
      image->height = 0;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_image(eir_gfx_image_t * image)
{
   if (image)
   {
      EIR_SYS_FREE(image->pixels);
      eir_gfx_init_image(image);
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_texture(eir_gfx_texture_t * texture)
{
   if (texture)
   {
      texture->id = EIR_GFX_INVALID_TEXTURE_HANDLE;
      texture->image = 0;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_texture(eir_gfx_texture_t * texture)
{
   if (texture)
   {
      eir_gfx_api_destroy_texture(texture->id);
      eir_gfx_init_texture(texture);
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_sprite_info(eir_gfx_sprite_info_t * sprite_info)
{
   if (sprite_info)
   {
      sprite_info->position.x = 0.0f;
      sprite_info->position.y = 0.0f;
      sprite_info->size.x = 0.0f;
      sprite_info->size.y = 0.0f;
      sprite_info->uv_offset.x = 0.0f;
      sprite_info->uv_offset.y = 0.0f;
      sprite_info->uv_size.x = 0.0f;
      sprite_info->uv_size.y = 0.0f;
      sprite_info->texture = 0;
      sprite_info->color.r = 0.0f;
      sprite_info->color.g = 0.0f;
      sprite_info->color.b = 0.0f;
      sprite_info->color.a = 0.0f;
      sprite_info->info = eir_gfx_item_flag_default;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_sprite_info(eir_gfx_sprite_info_t * sprite_info)
{
   eir_gfx_init_sprite_info(sprite_info);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_sprite_proxy(eir_gfx_sprite_proxy_t * sprite_proxy)
{
   if (sprite_proxy)
   {
      sprite_proxy->sprite = 0;
      sprite_proxy->sprite_info = 0;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_sprite_proxy(eir_gfx_sprite_proxy_t * sprite_proxy)
{
   eir_gfx_init_sprite_proxy(sprite_proxy);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_sprite(eir_gfx_sprite_t * sprite)
{
   if (sprite)
   {
      sprite->position.x = 0.0f;
      sprite->position.y = 0.0f;
      sprite->size.x = 0.0f;
      sprite->size.y = 0.0f;
      sprite->uv_offset.x = 0.0f;
      sprite->uv_offset.y = 0.0f;
      sprite->uv_size.x = 0.0f;
      sprite->uv_size.y = 0.0f;
      sprite->batch = 0;
      sprite->color.r = 0.0f;
      sprite->color.g = 0.0f;
      sprite->color.b = 0.0f;
      sprite->color.a = 0.0f;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_sprite(eir_gfx_sprite_t * sprite)
{
   eir_gfx_init_sprite(sprite);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   if (batch)
   {
      EIR_KER_INIT_ARRAY(batch->sprites);
      batch->vbo = EIR_GFX_INVALID_VBO_HANDLE;
      batch->vao = EIR_GFX_INVALID_VAO_HANDLE;
      batch->texture = 0;
      batch->program = EIR_GFX_INVALID_PROGRAM_HANDLE;
      batch->info = eir_gfx_item_flag_default;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   if (batch)
   {
      eir_gfx_api_release_sprite_batch(batch);
      EIR_KER_FREE_ARRAY_BIS(batch->sprites, eir_gfx_release_sprite);
      eir_gfx_init_sprite_batch(batch);
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_text_info(eir_gfx_text_info_t * text_info)
{
   if (text_info)
   {
      eir_ker_init_str(&text_info);
      text_info->position.x = 0.0f;
      text_info->position.y = 0.0f;
      text_info->font_size = 0.0f;
      text_info->color.r = 0.0f;
      text_info->color.g = 0.0f;
      text_info->color.b = 0.0f;
      text_info->color.a = 0.0f;
      text_info->info = eir_gfx_item_flag_default;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_text_info(eir_gfx_text_info_t * text_info)
{
   eir_gfx_init_text_info(text_info);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_text_proxy(eir_gfx_text_proxy_t * text_proxy)
{
   if (text_proxy)
   {
      text_proxy->text = 0;
      text_proxy->text_info = 0;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_text_proxy(eir_gfx_text_proxy_t * text_proxy)
{
   eir_gfx_init_text_proxy(text_proxy);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_text(eir_gfx_text_t * text)
{
   eir_gfx_init_sprite_batch(text);
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_text(eir_gfx_text_t * text)
{
   eir_gfx_init_text(text);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_rect_info(eir_gfx_rect_info_t * rect_info)
{
   if (rect_info)
   {
      rect_info->position.x = 0.0f;
      rect_info->position.y = 0.0f;
      rect_info->size.x = 0.0f;
      rect_info->size.y = 0.0f;
      rect_info->color.r = 0.0f;
      rect_info->color.g = 0.0f;
      rect_info->color.b = 0.0f;
      rect_info->color.a = 0.0f;
      rect_info->info = eir_gfx_item_flag_default;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_rect_info(eir_gfx_rect_info_t * rect_info)
{
   eir_gfx_init_rect_info(rect_info);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_rect_proxy(eir_gfx_rect_proxy_t * rect_proxy)
{
   if (rect_proxy)
   {
      rect_proxy->rect = 0;
      rect_proxy->rect_info = 0;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_rect_proxy(eir_gfx_rect_proxy_t * rect_proxy)
{
   eir_gfx_init_rect_proxy(rect_proxy);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_rect(eir_gfx_rect_t * rect)
{
   if (rect)
   {
      rect->position.x = 0.0f;
      rect->position.x = 0.0f;
      rect->size.x = 0.0f;
      rect->size.x = 0.0f;
      rect->color.r = 0.0f;
      rect->color.g = 0.0f;
      rect->color.b = 0.0f;
      rect->color.a = 0.0f;
      rect->batch = 0;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_rect(eir_gfx_rect_t * rect)
{
   eir_gfx_init_rect(rect);
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_rect_batch(eir_gfx_rect_batch_t * batch)
{
   if (batch)
   {
      EIR_KER_INIT_ARRAY(batch->rects);
      batch->vbo = EIR_GFX_INVALID_VBO_HANDLE;
      batch->vao = EIR_GFX_INVALID_VAO_HANDLE;
      batch->program = EIR_GFX_INVALID_PROGRAM_HANDLE;
      batch->info = eir_gfx_item_flag_default;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_rect_batch(eir_gfx_rect_batch_t * batch)
{
   if (batch)
   {
      eir_gfx_api_release_rect_batch(batch);
      EIR_KER_FREE_ARRAY(batch->rects);
      eir_gfx_init_rect_batch(batch);
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_init_group(eir_gfx_group_t * group)
{
   if (group)
   {
      EIR_KER_INIT_ARRAY(group->sprite_infos);
      EIR_KER_INIT_ARRAY(group->sprite_proxies);
      EIR_KER_INIT_ARRAY(group->sprite_batches);
      EIR_KER_INIT_ARRAY(group->text_infos);
      EIR_KER_INIT_ARRAY(group->text_proxies);
      EIR_KER_INIT_ARRAY(group->texts);
      EIR_KER_INIT_ARRAY(group->rect_infos);
      EIR_KER_INIT_ARRAY(group->rect_proxies);
      EIR_KER_INIT_ARRAY(group->rect_batches);
      group->visible = true;
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_release_group(eir_gfx_group_t * group)
{
   if (group)
   {
      EIR_KER_FREE_ARRAY_BIS(
         group->sprite_infos,
         eir_gfx_release_sprite_info
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->sprite_proxies,
         eir_gfx_release_sprite_proxy
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->sprite_batches,
         eir_gfx_release_sprite_batch
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->text_infos,
         eir_gfx_release_text_info
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->text_proxies,
         eir_gfx_release_text_proxy
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->text_batches,
         eir_gfx_release_text
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->rect_infos,
         eir_gfx_release_rect_info
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->rect_proxies,
         eir_gfx_release_rect_proxy
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->rect_batches,
         eir_gfx_release_rect_batch
         );
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_init_env(eir_gfx_env_t * gfx_env, int width, int height)
{
   EIR_KER_LOG_MESSAGE("init graphics env");
   if (gfx_env)
   {
      gfx_env->viewport.left = 0;
      gfx_env->viewport.right = width;
      gfx_env->viewport.bottom = height;
      gfx_env->viewport.top = 0;
      eir_mth_set_ortho_mat4(&gfx_env->projection, 0, width, height, 0);
      eir_mth_set_identity_mat4(&gfx_env->view);
      EIR_KER_INIT_ARRAY(gfx_env->images);
      EIR_KER_INIT_ARRAY(gfx_env->textures);
      EIR_KER_INIT_ARRAY(gfx_env->groups);
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_release_env(eir_gfx_env_t * gfx_env)
{
   EIR_KER_LOG_MESSAGE("release graphics env");
   if (gfx_env)
   {
      eir_gfx_api_destroy_texture(gfx_env->text_texture.id);
      EIR_SYS_FREE(gfx_env->text_image.pixels);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->images, eir_gfx_release_image);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->textures, eir_gfx_release_texture);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->groups, eir_gfx_release_group);
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_image_capacity(eir_gfx_env_t * env, size_t max_count)
{
   eir_gfx_env_t * gfx_env = 0;

   if (env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_image_t,
         env->images,
         max_count,
         eir_gfx_init_image
         );
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_texture_capacity(eir_gfx_env_t * env, size_t max_count)
{
   if (env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_texture_t,
         env->textures,
         max_count,
         eir_gfx_init_texture
         );
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_group_capacity(eir_gfx_env_t * env, size_t max_count)
{
   if (env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_group_t,
         env->groups,
         max_count,
         eir_gfx_init_group
         );
   }
}

// ---------------------------------------------------------------------------
eir_gfx_image_t * eir_gfx_load_image(
   eir_gfx_env_t * env,
   const char * img_filename,
   bool must_invert_img
   )
{
   eir_gfx_image_t * image = 0;

   if (env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->images, image);
   }
   if (image)
   {
      bool result = eir_gfx_api_load_image(
         img_filename,
         must_invert_img,
         image
         );

      if (!result)
      {
         EIR_KER_LOG_ERROR("cannot load image from file %s", img_filename);
         eir_gfx_release_image(image);
         EIR_KER_FREE_ARRAY_LAST_RESERVED_SLOT(env->images);
         image = 0;
      }
      else
      {
         EIR_KER_LOG_MESSAGE("%s has been loaded", img_filename);
      }
   }
   return image;
}

// ---------------------------------------------------------------------------
eir_gfx_texture_t * eir_gfx_create_texture(
   eir_gfx_env_t * env,
   const eir_gfx_image_t * image
   )
{
   eir_gfx_texture_t * texture = 0;

   if (env && image)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->textures, texture);
   }
   if (texture)
   {
      texture->id = eir_gfx_api_create_texture(image);
      texture->image = image;
      if (texture->id == EIR_GFX_INVALID_TEXTURE_HANDLE)
      {
         EIR_KER_LOG_ERROR(
            "create texture from image has failed: release texture slot"
            );
         eir_gfx_release_texture(texture);
         EIR_KER_FREE_ARRAY_LAST_RESERVED_SLOT(env->textures);
      }
   }
   return texture;
}

/*
int eir_gfx_add_text(
   eir_gfx_env_t * gfx_env,
   const char * text,
   eir_mth_vec2_t * position,
   float font_size,
   eir_gfx_color_t * color
   )
{
   if (!gfx_env || !text || !position || !color)
   {
      return -1;
   }

   const float MAX_TEXTURE_WIDTH = 320;
   const float MAX_TEXTURE_HEIGHT = 320;
   const float MAX_TEXTURE_COL = 16;
   const float MAX_TEXTURE_ROW = 16;

   int text_len = strlen(text);
   int batch_handle = -1;
   eir_gfx_sprite_batch_t * batch = 0;

   EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->text_batches, batch, batch_handle);
   if (batch)
   {
      EIR_KER_ALLOCATE_ARRAY(eir_gfx_sprite_t, batch->sprites, text_len);
      batch->built = false;
      batch->modified = true;
   }

   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_mth_vec2_t size;
   char c;
   float x_offset;
   float y_offset;

   size.x = 0.9f < font_size ? font_size : (MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL) / 4.0f;
   size.y = 0.9f < font_size ? font_size : (MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW) / 4.0f;
   for (int index = 0; index < text_len; ++index)
   {
      float x = position->x + size.x * index;
      float y = position->y;

      uv_size.x = MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL;
      uv_size.y = MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW;
      c = text[index];
      x_offset = (int)c % (int)MAX_TEXTURE_COL;
      y_offset = (int)c / (int)MAX_TEXTURE_ROW;  
      uv_offset.x = x_offset * (MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL);
      uv_offset.y = y_offset * (MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW);

      eir_gfx_sprite_t * sprite = 0;

      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->sprites, sprite);
      if (sprite)
      {
    sprite->position.x = x;
    sprite->position.y = y;
    sprite->size.x = size.x;
    sprite->size.y = size.y;
    sprite->uv_offset.x = uv_offset.x;
    sprite->uv_offset.y = uv_offset.y;
    sprite->uv_size.x = uv_size.x;
    sprite->uv_size.y = uv_size.y;
    sprite->color.r = color->r;
    sprite->color.g = color->g;
    sprite->color.b = color->b;
    sprite->color.a = color->a;
    sprite->batch = batch;
    sprite->visible = true;
      }
   }
   return batch_handle;
}
*/

/*
void eir_gfx_force_update_all_batches(eir_gfx_env_t * gfx_env)
{
   if (gfx_env)
   {
      for (int index = 0; index < gfx_env->sprite_batches.used; ++index)
      {
    gfx_env->sprite_batches.data[index].modified = true;
      }
      gfx_env->rect_batch.modified = true;
   }
}
*/

// ---------------------------------------------------------------------------
eir_gfx_group_t * eir_gfx_create_group(
   eir_gfx_env_t * env,
   size_t sprite_capacity,
   size_t text_capacity,
   size_t rect_capacity
   )
{
   eir_gfx_group_t * group = 0;

   if (env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->groups, group);
   }
   if (group)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_sprite_info_t,
         group->sprite_infos,
         sprite_capacity,
         eir_gfx_init_sprite_info
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_sprite_proxy_t,
         group->sprite_proxies,
         sprite_capacity,
         eir_gfx_init_sprite_proxy
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_text_info_t,
         group->text_infos,
         text_capacity,
         eir_gfx_init_text_info
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_text_proxy_t,
         group->text_proxies,
         text_capacity,
         eir_gfx_init_text_proxy
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_rect_info_t,
         group->rect_infos,
         rect_capacity,
         eir_gfx_init_rect_info
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_rect_proxy_t,
         group->rect_proxies,
         rect_capacity,
         eir_gfx_init_rect_proxy
         );
      group->sprite_batches
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot get another empty group");
   }
   return group;
}

// ---------------------------------------------------------------------------
eir_gfx_sprite_proxy_t * eir_gfx_push_sprite_to_group(
   eir_gfx_group_t * group,
   const eir_gfx_texture_t * texture,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_mth_vec2_t * uv_offset,
   const eir_mth_vec2_t * uv_size,
   const eir_gfx_color_t * color,
   bool editable,
   bool use_screen_coord,
   bool visible
   )
{
   eir_gfx_sprite_info_t * sprite_info = 0;
   eir_gfx_sprite_proxy_t * sprite_proxy = 0;

   if (group)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(group->sprite_infos, sprite_info);
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(group->sprite_proxies, sprite_proxy);
   }
   if (sprite_info && sprite_proxy)
   {
      sprite_info->position.x = position->x;
      sprite_info->position.y = position->y;
      sprite_info->size.x = size->x;
      sprite_info->size.y = size->y;
      sprite_info->uv_offset.x = uv_offset->x;
      sprite_info->uv_offset.y = uv_offset->y;
      sprite_info->uv_size.x = uv_size->x;
      sprite_info->uv_size.y = uv_size->y;
      sprite_info->texture = texture;
      sprite_info->color.r = color->r;
      sprite_info->color.g = color->g;
      sprite_info->color.b = color->b;
      sprite_info->color.a = color->a;
      if (editable)
      {
         sprite_info->info |= eir_gx_item_flag_editable;
      }
      if (use_screen_coord)
      {
         sprite_info->info |= eir_gfx_item_flag_use_screen_coord;
      }
      if (visible)
      {
         sprite_info->info |= eir_gfx_item_flag_visible;
      }
   }
   else
   {
      EIR_KER_LOG_MESSAGE("cannot get another empty sprite info or proxy");
   }
   return sprite_proxy;
}

// ---------------------------------------------------------------------------
eir_gfx_text_proxy_t * eir_gfx_push_text_to_group(
   eir_gfx_group_t * group,
   const char * text,
   const eir_mth_vec2_t * position,
   float font_size,
   const eir_gfx_color_t * color,
   bool editable,
   bool use_screen_coord,
   bool visible
   )
{
   eir_gfx_text_info_t * text_info = 0;
   eir_gfx_text_proxy_t * text_proxy = 0;

   if (group)
   {
      EIR_GFX_GET_ARRAY_NEXT_EMPTY_SLOT(group->text_infos, text_info);
      EIR_GFX_GET_ARRAY_NEXT_EMPTY_SLOT(group->text_proxies, text_proxy);
   }
   if (text_info && text_proxy)
   {
      eir_sys_mem_cpy(text_info->text, text, EIR_GFX_TEXT_INFO_BUFFER);
      text_info->position.x = position->x;
      text_info->position.y = position->y;
      text_info->font_size = font_size;
      text_info->color.r = color->r;
      text_info->color.g = color->g;
      text_info->color.b = color->b;
      text_info->color.a = color->a;
      if (editable)
      {
         text_info->info |= eir_gx_item_flag_editable;
      }
      if (use_screen_coord)
      {
         text_info->info |= eir_gfx_item_flag_use_screen_coord;
      }
      if (visible)
      {
         text_info->info |= eir_gfx_item_flag_visible;
      }
   }
   else
   {
      EIR_KER_LOG_MESSAGE("cannot get another empty text info or proxy");
   }
   return text_proxy;
}

// ---------------------------------------------------------------------------
eir_gfx_rect_t * eir_gfx_push_rect_to_group(
   eir_gfx_group_t * group,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_gfx_color_t * color,
   bool editable,
   bool use_screen_coord,
   bool visible
   )
{
   eir_gfx_rect_info_t * rect_info = 0;
   eir_gfx_rect_proxy_t * rect_proxy = 0;

   if (group)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(group->rect_infos, rect_info);
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(group->rect_proxies, rect_proxy);
   }
   if (rect_info && rect_proxy)
   {
      rect_info->position.x = position->x;
      rect_info->position.y = position->y;
      rect_info->size.x = size->x;
      rect_info->size.y = size->y;
      rect_info->color.r = color->r;
      rect_info->color.g = color->g;
      rect_info->color.b = color->b;
      rect_info->color.a = color->a;
      if (editable)
      {
         rect_info->info |= eir_gx_item_flag_editable;
      }
      if (use_screen_coord)
      {
         rect_info->info |= eir_gfx_item_flag_use_screen_coord;
      }
      if (visible)
      {
         rect_info->info |= eir_gfx_item_flag_visible;
      }
   }
   else
   {
      EIR_KER_LOG_MESSAGE("cannot get another empty rect info or proxy");
   }
   return rect_proxy;
}

// ---------------------------------------------------------------------------
void eir_gfx_set_group_visibility(
   eir_gfx_group_t * group,
   bool visible
   )
{
   if (group)
   {
      group->visible = visible;
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_modify_sprite(
   eir_gfx_sprite_proxy_t * gfx_sprite_proxy,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_mth_vec2_t * uv_offset,
   const eir_mth_vec2_t * uv_size,
   const eir_gfx_color_t * color
   )
{
   eir_gfx_sprite_t * sprite = 0;

   if (gfx_sprite_proxy)
   {
      sprite = gfx_sprite_proxy->sprite;
   }
   if (sprite)
   {
      sprite->position.x = position->x;
      sprite->position.y = position->y;
      sprite->size.x = size->x;
      sprite->size.y = size->y;
      sprite->uv_offset.x = uv_offset->x;
      sprite->uv_offset.y = uv_offset->y;
      sprite->color.r = color->r;
      sprite->color.g = color->g;
      sprite->color.b = color->b;
      sprite->color.a = color->a;
      sprite->batch->info |= eir_gfx_item_flag_modified;
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_modify_text(
   eir_gfx_text_proxy_t * gfx_text_proxy,
   const char * text,
   const eir_mth_vec2_t * position,
   float font_size,
   const eir_gfx_color_t * color
   )
{
   eir_gfx_text_t * gfx_text = 0;
   
   if (gfx_text_proxy)
   {
      gfx_text = gfx_text_proxy->text;
   }
   if (gfx_text)
   {
      EIR_KER_FREE_ARRAY(gfx_text->sprites);

      int text_len = strlen(text);

      EIR_KER_ALLOCATE_ARRAY(eir_gfx_sprite_t, gfx_text->sprites, text_len);

      const float MAX_TEXTURE_WIDTH = 320;
      const float MAX_TEXTURE_HEIGHT = 320;
      const float MAX_TEXTURE_COL = 16;
      const float MAX_TEXTURE_ROW = 16;
      char c;
      float x_offset;
      float y_offset;
      eir_mth_vec2_t uv_offset;
      eir_mth_vec2_t uv_size;
      eir_mth_vec2_t size;

      size.x =
         0.9f < font_size ?
         font_size : (MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL) / 4.0f;
      size.y =
         0.9f < font_size ?
         font_size : (MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW) / 4.0f;
      for (int index = 0; index < text_len; ++index)
      {
         float x = position->x + size.x * index;
         float y = position->y;

         uv_size.x = MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL;
         uv_size.y = MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW;
         c = text[index];
         x_offset = (int)c % (int)MAX_TEXTURE_COL;
         y_offset = (int)c / (int)MAX_TEXTURE_ROW;  
         uv_offset.x = x_offset * (MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL);
         uv_offset.y = y_offset * (MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW);

         eir_gfx_sprite_t * sprite = 0;

         EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(gfx_text->sprites, sprite);
         if (sprite)
         {
            sprite->position.x = x;
            sprite->position.y = y;
            sprite->size.x = size.x;
            sprite->size.y = size.y;
            sprite->uv_offset.x = uv_offset.x;
            sprite->uv_offset.y = uv_offset.y;
            sprite->uv_size.x = uv_size.x;
            sprite->uv_size.y = uv_size.y;
            sprite->color.r = color->r;
            sprite->color.g = color->g;
            sprite->color.b = color->b;
            sprite->color.a = color->a;
         }
      }
      gfx_text->info |= eir_gfx_item_flag_modified;
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_modify_rect(
   eir_gfx_rect_proxy_t * gfx_rect_proxy,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_gfx_color_t * color
   )
{
   eir_gfx_rect_t * rect = 0;

   if (gfx_rect_proxy)
   {
      rect = gfx_rect_proxy->rect;
   }
   if (rect)
   {
      rect->position.x = position->x;
      rect->position.y = position->y;
      rect->size.x = size->x;
      rect->size.y = size->y;
      rect->color.r = color->r;
      rect->color.g = color->g;
      rect->color.b = color->b;
      rect->color.a = color->a;
      rect->batch->info |= eir_gfx_item_flag_modified;
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_sprite_visibility(
   eir_gfx_sprite_proxy_t * gfx_sprite_proxy,
   bool visible
   )
{
   eir_gfx_sprite_t * sprite = 0;
   eir_gfx_sprite_info_t * sprite_info = 0;

   if (gfx_sprite_proxy)
   {
      sprite = gfx_sprite_proxy->sprite;
      sprite_info = gfx_sprite_proxy->sprite_info;
   }
   if (sprite && sprite_info)
   {
      if ((sprite_info->info & eir_gfx_item_flag_visible) && !visible)
      {
         sprite_info->info ~= eir_gfx_item_flag_visible;
         if (sprite->batch)
         {
            sprite->batch->info |= eir_gfx_item_flag_modified;
         }
      }
      else if (!(sprite_info & eir_gfx_item_flag_visible) && visible)
      {
         sprite_info->info |= eir_gfx_item_flag_visible;
         if (sprite->batch)
         {
            sprite->batch->info |= eir_gfx_item_flag_modified;
         }
      }
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_text_visibility(
   eir_gfx_text_proxy_t * gfx_text_proxy,
   bool visible
   )
{
   eir_gfx_text_t * text = 0;
   eir_gfx_text_info_t * text_info = 0;

   if (gfx_text_proxy)
   {
      text = gfx_text_proxy->text;
      text_info = gfx_text_proxy->text_info;
   }
   if (text && text_info)
   {
      if ((text_info->info & eir_gfx_item_flag_visible) && !visible)
      {
         text_info->info ~= eir_gfx_item_flag_visible;
         text->info |= eir_gfx_item_flag_modified;
      }
      else if (!(text_info & eir_gfx_item_flag_visible) && visible)
      {
         text_info->info |= eir_gfx_item_flag_visible;
         text->info |= eir_gfx_item_flag_modified;
      }
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_rect_visibility(
   eir_gfx_rect_proxy_t * gfx_rect_proxy,
   bool visible
   )
{
   eir_gfx_rect_t * rect = 0;
   eir_gfx_rect_info_t * rect_info = 0;

   if (gfx_rect_proxy)
   {
      rect = gfx_rect_proxy->rect;
      rect_info = gfx_rect_proxy->rect_info;
   }
   if (rect && rect_info)
   {
      if ((rect_info->info & eir_gfx_item_flag_visible) && !visible)
      {
         rect_info->info ~= eir_gfx_item_flag_visible;
         if (rect->batch)
         {
            rect->batch->info |= eir_gfx_item_flag_modified;
         }
      }
      else if (!(rect_info & eir_gfx_item_flag_visible) && visible)
      {
         rect_info->info |= eir_gfx_item_flag_visible;
         if (rect->batch)
         {
            rect->batch->info |= eir_gfx_item_flag_modified;
         }
      }
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_sprite_batch(
   eir_gfx_env_t * env,
   eir_gfx_sprite_batch_t * batch
   )
{
   // TODO
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_text(
   eir_gfx_env_t * env,
   eir_gfx_text_t * text
   )
{
   // TODO
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_rect_batch(
   eir_gfx_env_t * env,
   eir_gfx_rect_batch_t * batch
   )
{
   // TODO
}

// ---------------------------------------------------------------------------
static eir_gfx_sprite_batch_t * eir_gfx_create_sprite_batch(
   eir_gfx_env_t * gfx_env,
   int max_capacity,
   bool editable,
   bool visible
   )
{
   eir_gfx_sprite_batch_t * batch = 0;

   if (gfx_env && max_capacity > 0)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(gfx_env->sprite_batches, batch);
   }
   if (batch)
   {
      EIR_KER_ALLOCATE_ARRAY(eir_gfx_sprite_t, batch->sprites, max_capacity);
      batch->info = eir_gfx_item_flag_default;
      if (editable)
      {
         batch->info |= eir_gfx_item_flag_editable;
      }
      if (visible)
      {
         batch->info |= eir_gfx_item_flag_visible;
      }
   }
   return batch;
}

// ---------------------------------------------------------------------------
static eir_gfx_sprite_t * eir_gfx_add_sprite_to_batch(
   eir_gfx_sprite_batch_t * batch,
   const eir_gfx_sprite_info_t * sprite_info;
   )
{
   eir_gfx_sprite_t * sprite = 0;

   if (batch)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->sprites, sprite);
   }
   if (sprite && sprite_info)
   {
      sprite_info->info |= eir_gfx_item_flag_added;
      sprite->info = sprite_info->info;

      const eir_mth_vec2_t * position = &sprite_info->position;
      const eir_mth_vec2_t * size = &sprite_info->size;
      const eir_mth_vec2_t * uv_offset = &sprite_info->uv_offset;
      const eir_mth_vec2_t * uv_size = &sprite_info->uv_size;
      const eir_gfx_color_t * color = &sprite_info->color;

      sprite->position.x = position->x;
      sprite->position.y = position->y;
      sprite->size.x = size->x;
      sprite->size.y = size->y;
      sprite->uv_offset.x = uv_offset->x;
      sprite->uv_offset.y = uv_offset->y;
      sprite->uv_size.x = uv_size->x;
      sprite->uv_size.y = uv_size->y;
      sprite->color.r = color->r;
      sprite->color.g = color->g;
      sprite->color.b = color->b;
      sprite->color.a = color->a;
      sprite->batch = batch;
   }
   return sprite;
}

// ---------------------------------------------------------------------------
static eir_gfx_rect_t * eir_gfx_add_rect_to_batch(
   eir_gfx_rect_batch_t * batch,
   const eir_gfx_rect_info_t * rect_info
   )
{
   eir_gfx_rect_t * rect = 0;

   if (batch && size && position && color)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->rects, rect);
   }
   if (rect)
   {
      rect_info->info |= eir_gfx_item_flag_added;
      rect->info |= rect_info->info;

      const eir_mth_vec2_t * position = &rect_info->position;
      const eir_mth_vec2_t * size = &rect_info->size;
      const eir_gfx_color_t * color = &rect_info->color;

      rect->position.x = position->x;
      rect->position.y = position->y;
      rect->size.x = size->x;
      rect->size.y = size->y;
      rect->color.r = color->r;
      rect->color.g = color->g;
      rect->color.b = color->b;
      rect->color.a = color->a;
      rect->batch = batch;
   }
   return rect;
}


// ---------------------------------------------------------------------------
static void eir_gfx_generate_group_sprite_batches(
   eir_gfx_env_t * env,
   eir_gfx_group_t * group
   )
{
   // TODO
}

// ---------------------------------------------------------------------------
static void eir_gfx_generate_group_text(
   eir_gfx_env_t * env,
   eir_gfx_group_t * group
   )
{
   // TODO
}

// ---------------------------------------------------------------------------
static void eir_gfx_generate_group_rect_batches(
   eir_gfx_env_t * env,
   eir_gfx_group_t * group
   )
{
   // TODO
}

// ---------------------------------------------------------------------------
static bool eir_gfx_must_add_item(eir_gfx_item_info_t info)
{
   return !(info & eir_gfx_item_flag_added)
      && (info & eir_gfx_item_flag_visible);
}

// ---------------------------------------------------------------------------
static bool eir_gfx_must_remove_item(eir_gfx_item_info_t info) 
{
   return (info & eir_gfx_item_flag_added)
      && !(info & eir_gfx_item_flag_visible);
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_group(eir_gfx_env_t * env, eir_gfx_group_t * group)
{
   if (env && group && group->visible)
   {
      int sprite_count = group->sprites_infos.used;
      bool regenerate_batches = false;
      
      for (int index = 0; index < sprite_count; ++index)
      {
         eir_gfx_sprite_info_t * sprite_info = &group->sprite_infos.data[index];

         if (
            eir_gfx_must_add_item(sprite_info->info)
            || eir_gfx_must_remove_item(sprite_info->info)
            )
         {
            regenerate_batches = true;
         }
      }
      if (regenerate_batches)
      {
         eir_gfx_generate_group_sprite_batches(env, group);
      }

      int text_count = group->text_infos.used;

      regenerate_batches = false;
      for (int index = 0; index < text_count; ++index)
      {
         eir_gfx_text_info_t * text_info = &group->text_infos.data[index];

         if (
            eir_gfx_must_add_item(text_info)
            || eir_gfx_must_remove_item(text_info->info)
            )
         {
            regenerate_batches = true;
         }
      }
      if (regenerate_batches)
      {
         eir_gfx_generate_group_texts(env, group);
      }
      
      int rect_count = group->rect_infos.used;
      
      regenerate_batches = false;
      for (int index = 0; index < rect_count; ++index)
      {
         eir_gfx_rect_info_t * rect_info = &group->rect_infos.data[index];

         if (
            eir_gfx_must_add_item(rect_info->info)
            || eir_gfx_must_remove_item(rect_info->info)
            )
         {
            regenerate_batches = true;
         }
      }
      if (regenerate_batches)
      {
         eir_gfx_generate_group_rect_batches(env, group);
      }

      for (int index = 0; index < group->sprite_batches.used; ++index)
      {
         eir_gfx_render_sprite_batch(env, &group->sprite_batches.data[index]);
      }
      for (int index = 0; index < group->texts; ++index)
      {
         eir_gfx_render_text_batch(env, &group->texts.data[index]);
      }
      for (int index = 0; index < group->rect_batches.used; ++index)
      {
         eir_gfx_render_rect_batch(env, &group->rect_batches.data[index]);
      }
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_render_all_batches(eir_gfx_env_t * gfx_env)
{
   for (int index = 0; index < gfx_env->groups.used; ++index)
   {
      eir_gfx_render_group(gfx_env, &gfx_env->groups.data[index]);
   }

/*
   if (!gfx_env)
   {
      return;
   }

   eir_gfx_sprite_batch_t * batch = 0;
   eir_mth_mat4_t id_mat;

   eir_mth_set_identity_mat4(&id_mat);
   for (int index = 0; index < gfx_env->sprite_batches.used; ++index)
   {
      EIR_KER_GET_ARRAY_ITEM(gfx_env->sprite_batches, index, batch);
      if (batch)
      {
         if (!batch->built)
         {
            eir_gfx_api_build_sprite_batch(gfx_env, batch);
         }
         else if (batch->modified)
         {
            eir_gfx_api_bind_vertex_array(batch->vao);
            eir_gfx_api_bind_array_buffer(batch->vbo);
            eir_gfx_api_set_sprite_buffer_data(batch);
            eir_gfx_api_unbind_vertex_array();
         }
         eir_gfx_api_bind_texture(batch->texture->id);
         eir_gfx_api_use_program(batch->program);
         eir_gfx_api_set_program_mat4("pmat", batch->program, &gfx_env->projection);
         eir_gfx_api_set_program_mat4("vmat", batch->program, &gfx_env->view);
         eir_gfx_api_draw_sprite_batch(batch);
         eir_gfx_api_unuse_program();
      }
   }
   for (int index = 0; index < gfx_env->text_batches.used; ++index)
   {
      EIR_KER_GET_ARRAY_ITEM(gfx_env->text_batches, index, batch);
      if (batch)
      {
         if (!batch->built)
         {
            eir_gfx_api_build_text_batch(gfx_env, batch);
         }
         else if (batch->modified)
         {
            eir_gfx_api_bind_vertex_array(batch->vao);
            eir_gfx_api_bind_array_buffer(batch->vbo);
            eir_gfx_api_set_sprite_buffer_data(batch);
            eir_gfx_api_unbind_vertex_array();
         }
         eir_gfx_api_bind_texture(batch->texture->id);
         eir_gfx_api_use_program(batch->program);
         eir_gfx_api_set_program_mat4("pmat", batch->program, &gfx_env->projection);
         eir_gfx_api_set_program_mat4("vmat", batch->program, &id_mat);
         eir_gfx_api_draw_sprite_batch(batch);
         eir_gfx_api_unuse_program();
      }
   }
   if (0 < gfx_env->rect_batch.rects.used)
   {
      if (!gfx_env->rect_batch.built)
      {
         eir_gfx_api_build_rect_batch(gfx_env, &gfx_env->rect_batch);
      }
      else if (gfx_env->rect_batch.modified)
      {
         eir_gfx_api_bind_vertex_array(gfx_env->rect_batch.vao);
         eir_gfx_api_bind_array_buffer(gfx_env->rect_batch.vbo);
         eir_gfx_api_set_rect_buffer_data(&gfx_env->rect_batch);
         eir_gfx_api_unbind_vertex_array();
      }
      eir_gfx_api_use_program(gfx_env->rect_batch.program);
      eir_gfx_api_set_program_mat4("rect_pmat", gfx_env->rect_batch.program, &gfx_env->projection);
      eir_gfx_api_set_program_mat4("rect_vmat", gfx_env->rect_batch.program, &gfx_env->view);
      eir_gfx_api_draw_rect_batch(&gfx_env->rect_batch);
      eir_gfx_api_unuse_program();
   }
   eir_gfx_api_check_for_error();
   */
}

/*
void eir_gfx_update_text(
   eir_gfx_env_t * gfx_env,
   int text_index,
   const char * text
   )
{
   if (!gfx_env || 0 > text_index || !text)
   {
      return;
   }

   eir_gfx_sprite_batch_t * batch = 0;
   EIR_KER_GET_ARRAY_ITEM(gfx_env->text_batches, text_index, batch);

   if (!batch)
   {
      return;
   }
   
   eir_mth_vec2_t position;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_mth_vec2_t size;
   eir_gfx_color_t color;

   position.x = batch->sprites.data[0].position.x;
   position.y = batch->sprites.data[0].position.y;
   size.x = batch->sprites.data[0].size.x;
   size.y = batch->sprites.data[0].size.x;
   uv_offset.x = batch->sprites.data[0].uv_offset.x;
   uv_offset.y = batch->sprites.data[0].uv_offset.x;
   uv_size.x = batch->sprites.data[0].uv_size.x;
   uv_size.y = batch->sprites.data[0].uv_size.y;

   color.r = batch->sprites.data[0].color.r;
   color.g = batch->sprites.data[0].color.g;
   color.b = batch->sprites.data[0].color.b;
   color.a = batch->sprites.data[0].color.a;

   EIR_KER_FREE_ARRAY(batch->sprites);

   int text_len = strlen(text);

   EIR_KER_ALLOCATE_ARRAY(eir_gfx_sprite_t, batch->sprites, text_len);
   batch->modified = true;

   const float MAX_TEXTURE_WIDTH = 320;
   const float MAX_TEXTURE_HEIGHT = 320;
   const float MAX_TEXTURE_COL = 16;
   const float MAX_TEXTURE_ROW = 16;
   char c;
   float x_offset;
   float y_offset;

   for (int index = 0; index < text_len; ++index)
   {
      float x = position.x + size.x * index;
      float y = position.y;

      uv_size.x = MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL;
      uv_size.y = MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW;
      c = text[index];
      x_offset = (int)c % (int)MAX_TEXTURE_COL;
      y_offset = (int)c / (int)MAX_TEXTURE_ROW;  
      uv_offset.x = x_offset * (MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL);
      uv_offset.y = y_offset * (MAX_TEXTURE_HEIGHT / MAX_TEXTURE_ROW);

      eir_gfx_sprite_t * sprite = 0;

      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->sprites, sprite);
      if (sprite)
      {
    sprite->position.x = x;
    sprite->position.y = y;
    sprite->size.x = size.x;
    sprite->size.y = size.y;
    sprite->uv_offset.x = uv_offset.x;
    sprite->uv_offset.y = uv_offset.y;
    sprite->uv_size.x = uv_size.x;
    sprite->uv_size.y = uv_size.y;
    sprite->color.r = color.r;
    sprite->color.g = color.g;
    sprite->color.b = color.b;
    sprite->color.a = color.a;
      }
   }
}
*/

/*
eir_gfx_sprite_ref_t * eir_gfx_create_sprite_ref(eir_gfx_env_t * env, const eir_gfx_texture_t * texture, int img_x_offset, int img_y_offset, int img_width_offset, int img_height_offset)
{
   eir_gfx_sprite_ref_t * sprite_ref = 0;
      
   if (env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->sprites_ref, sprite_ref);
   }
   if (sprite_ref && texture)
   {
      sprite_ref->uv_offset.x = (float)img_x_offset;
      sprite_ref->uv_offset.y = (float)img_y_offset;
      sprite_ref->uv_size.x = (float)img_width_offset;
      sprite_ref->uv_size.y = (float)img_height_offset;
      sprite_ref->texture = texture;
   }
   return sprite_ref;
}
*/

/*
void eir_gfx_generate_all_batches(eir_gfx_env_t * gfx_env, const eir_gme_world_t * world)
{
   EIR_KER_LOG_MESSAGE("generate all batches from specified world");
   if (gfx_env && world)
   {
      size_t needed_batch_count = 0;
      eir_gme_entity_flags_t * entity = 0;

      EIR_KER_LOG_MESSAGE("%d entities found", world->entities_flags.used);

      for (int i = 0; i < world->entities_flags.used; ++i)
      {
    EIR_KER_LOG_MESSAGE("entity %d flags = %d", i, world->entities_flags.data[i]);
    if (world->entities_flags.data[i] & eir_gme_component_type_sprite)
    {
       EIR_KER_LOG_MESSAGE("entity %d has sprite", i);

       int j;

       for (j = 0; j < i; ++j)
       {
          if (world->sprite_refs.data[i].ptr == world->sprite_refs.data[j].ptr)
          {
        break;
          }
       }
       if (j == i)
       {
          ++needed_batch_count;
       }
    }
      }

      if (needed_batch_count == 0)
      {
    EIR_KER_LOG_MESSAGE("no need to generate batch. 0 sprite entities found")
    return;
      }

      EIR_KER_ALLOCATE_ARRAY_BIS(
    eir_gfx_sprite_batch_t,
    gfx_env->sprite_batches,
    needed_batch_count,
    eir_gfx_init_sprite_batch
    );

      for (int entity_index = 0; entity_index < world->entities_flags.used; ++entity_index)
      {
    EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity_index, entity);
    if (entity && ((*entity) & eir_gme_component_type_sprite))
    {
       int batch_item_count = 1;
       eir_gme_sprite_ref_component_t * sprite_ref_component = 0;

       EIR_KER_GET_ARRAY_ITEM(world->sprite_refs, entity_index, sprite_ref_component);
       if (sprite_ref_component)
       {
          eir_gfx_sprite_ref_t * sprite_ref = sprite_ref_component->ptr;
          
          if (!sprite_ref)
          {
        EIR_KER_LOG_ERROR("cannot get sprite from entity %d", entity_index);
        continue;
          }

          eir_gfx_sprite_batch_t * batch = 0;

          for (int batch_index = 0; batch_index < gfx_env->sprite_batches.used; ++batch_index)
          {
        eir_gfx_sprite_batch_t * batch_iter = 0;

        EIR_KER_GET_ARRAY_ITEM(
           gfx_env->sprite_batches,
           batch_index,
           batch_iter
           );

        if (batch_iter && batch_iter->texture == sprite_ref->texture)
        {
           batch = batch_iter;
           break;
        }
          }
          if (!batch)
          {
        int sibling_entity_index = entity_index + 1;
        eir_gme_sprite_ref_component_t * sibling_sprite_ref_component = 0;

        EIR_KER_GET_ARRAY_ITEM(
           world->sprite_refs,
           sibling_entity_index,
           sibling_sprite_ref_component
           );
        
        while (sibling_sprite_ref_component)
        {
           if (sibling_sprite_ref_component->ptr != 0 && sprite_ref_component->ptr == sprite_ref)
           {
         ++batch_item_count;
           }
           EIR_KER_GET_ARRAY_ITEM(
         world->sprite_refs,
         ++sibling_entity_index,
         sibling_sprite_ref_component
         );
        }
        batch = eir_gfx_create_sprite_batch(gfx_env, batch_item_count);
          }

          if (!batch)
          {
        EIR_KER_LOG_ERROR("cannot create or get sprite batch");
        continue;
          }

          EIR_KER_LOG_MESSAGE(
        "create batch for %d sprite(s) with texture id = %d",
        batch_item_count,
        sprite_ref->texture->id
        );

          batch->texture = sprite_ref->texture;

          eir_gme_position_component_t * position = 0;
          eir_gme_size_component_t * size = 0;
          eir_gme_color_component_t * color = 0;

          EIR_KER_GET_ARRAY_ITEM(
        world->positions,
        entity_index,
        position
        );
          EIR_KER_GET_ARRAY_ITEM(
        world->sizes,
        entity_index,
        size
        );
          EIR_KER_GET_ARRAY_ITEM(
        world->colors,
        entity_index,
        color
        );

          if (sprite_ref && position && size && color)
          {
        eir_gfx_sprite_t * sprite = eir_gfx_add_sprite_to_batch(
           batch,
           &position->initial,
           &size->initial,
           &sprite_ref->uv_offset,
           &sprite_ref->uv_size,
           &color->initial
           );
        position->current = &sprite->position;
        size->current = &sprite->size;
          }
       }
    }
      }

      for (int entity_index = 0; entity_index < world->entities_flags.used; ++entity_index)
      {
    EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity_index, entity);
    if (entity && ((*entity) & eir_gme_component_type_aabb))
    {
       eir_gme_aabb_component_t * aabb = 0;

            EIR_KER_GET_ARRAY_ITEM(world->aabbs, entity_index, aabb);
       if (aabb)
       {
          eir_gfx_color_t color;
          eir_mth_vec2_t position;
          eir_mth_vec2_t size;

          color.r = 1.0f;
          color.g = 1.0f;
          color.b = 0.0f;
          color.a = 0.3f;
          position.x = aabb->x_offset;
          position.y = aabb->y_offset;
          size.x = aabb->width;
          size.y = aabb->height;

          aabb->rect =  eir_gfx_add_rect(gfx_env, &position, &size, &color);

          EIR_KER_LOG_MESSAGE(
        "entity has aabb. add debug rect (%f; %f; %f; %f)",
        aabb->x_offset,
        aabb->y_offset,
        aabb->width,
        aabb->height
        );
       }
    }
    if (entity && ((*entity) & eir_gme_component_type_camera))
    {
            eir_gme_camera_component_t * camera = 0;;
            EIR_KER_GET_ARRAY_ITEM(world->cameras, entity_index, camera);

            if (camera && camera->target)
            {
               eir_gfx_color_t color;
               eir_mth_vec2_t position;
               eir_mth_vec2_t size;
               
               color.r = 1.0f;
               color.g = 0.5f;
               color.b = 0.0f;
               color.a = 0.3f;
               size.x = camera->target->width * camera->win_scale;
               size.y = camera->target->height * camera->win_scale;
               position.x = camera->target->x_offset - (size.x - camera->target->width) * 0.5f;
               position.y = camera->target->y_offset - (size.y - camera->target->height) * 0.5f;
               
               camera->win_rect =  eir_gfx_add_rect(gfx_env, &position, &size, &color);
               
               EIR_KER_LOG_MESSAGE(
                     "camera found. add window debug rect (%f; %f; %f; %f)",
                     camera->win_rect->position.x,
                     camera->win_rect->position.y,
                     camera->win_rect->size.x,
                     camera->win_rect->size.y
                     );
            }
    }
      }
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot generate all batches : gfx env or world are empty");
   }
}
*/
   
// ---------------------------------------------------------------------------
void eir_gfx_update_camera_view(
   eir_gfx_env_t * gfx_env,
   const eir_mth_vec2_t * cam_pos
   )
{
   if (gfx_env && cam_pos)
   {
      eir_mth_mat4_t * view = &gfx_env->view;
      eir_mth_vec3_t v;

      eir_mth_set_vec3(&v, cam_pos->x, cam_pos->y, 1.0f);
      eir_mth_set_translation_mat4(view, &v);
   }
}

