// ----------------------------------------------------------------------------
#include "eir_gfx_func.h"
#include "eir_gfx_api_func.h"
#include "../maths/eir_mth_func.h"

// ----------------------------------------------------------------------------
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
static void eir_gfx_init_sprite_proxy(eir_gfx_sprite_proxy_t * sprite_proxy)
{
   if (sprite_proxy)
   {
      sprite_proxy->position.x = 0.0f;
      sprite_proxy->position.y = 0.0f;
      sprite_proxy->size.x = 0.0f;
      sprite_proxy->size.y = 0.0f;
      sprite_proxy->uv_offset.x = 0.0f;
      sprite_proxy->uv_offset.y = 0.0f;
      sprite_proxy->uv_size.x = 0.0f;
      sprite_proxy->uv_size.y = 0.0f;
      sprite_proxy->texture = 0;
      sprite_proxy->color.r = 0.0f;
      sprite_proxy->color.g = 0.0f;
      sprite_proxy->color.b = 0.0f;
      sprite_proxy->color.a = 0.0f;
      sprite_proxy->info = eir_gfx_item_flag_default;
      sprite_proxy->buffer = 0;
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
static void eir_gfx_init_rect_proxy(eir_gfx_rect_proxy_t * rect_proxy)
{
   if (rect_proxy)
   {
      rect_proxy->position.x = 0.0f;
      rect_proxy->position.y = 0.0f;
      rect_proxy->size.x = 0.0f;
      rect_proxy->size.y = 0.0f;
      rect_proxy->color.r = 0.0f;
      rect_proxy->color.g = 0.0f;
      rect_proxy->color.b = 0.0f;
      rect_proxy->color.a = 0.0f;
      rect_proxy->info = eir_gfx_item_flag_default;
      rect_proxy->buffer = 0;
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
      EIR_KER_INIT_ARRAY(group->sprite_batches);
      EIR_KER_INIT_ARRAY(group->texts);
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
         group->sprite_batches,
         eir_gfx_release_sprite_batch
         );
      EIR_KER_FREE_ARRAY_BIS(
         group->texts,
         eir_gfx_release_text
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

// ---------------------------------------------------------------------------
eir_gfx_group_t * eir_gfx_create_group(
   eir_gfx_env_t * env,
   size_t sprite_batch_capacity,
   size_t text_capacity,
   size_t rect_batch_capacity
   )
{
   eir_gfx_group_t * group = 0;

   if (env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->groups, group);
   }
   if (group)
   {
      if (sprite_batch_capacity > 0)
      {
         EIR_KER_ALLOCATE_ARRAY_BIS(
            eir_gfx_sprite_batch_t,
            group->sprite_batches,
            sprite_batch_capacity,
            eir_gfx_init_sprite_batch
            );
      }
      if (text_capacity > 0)
      {
         EIR_KER_ALLOCATE_ARRAY_BIS(
            eir_gfx_text_t,
            group->texts,
            text_capacity,
            eir_gfx_init_text
            );
      }
      if (rect_batch_capacity)
      {
         EIR_KER_ALLOCATE_ARRAY_BIS(
            eir_gfx_rect_batch_t,
            group->rect_batches,
            rect_batch_capacity,
            eir_gfx_init_rect_batch
            );
      }
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot get another empty group");
   }
   return group;
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
eir_gfx_sprite_batch_t * eir_gfx_add_sprite_batch_to_group(
   eir_gfx_group_t * group,
   const eir_gfx_texture_t * texture,
   size_t sprite_count,
   bool editable,
   bool use_screen_coord,
   bool visible
   )
{
   eir_gfx_sprite_batch_t * batch = 0;

   if (group && sprite_count > 0)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(group->sprite_batches, batch);
   }
   if (batch)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_sprite_t,
         batch->sprites,
         sprite_count,
         eir_gfx_init_sprite
         )
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_sprite_proxy_t,
         batch->sprites_proxies,
         sprite_count,
         eir_gfx_init_sprite_proxy
         )
      batch->texture = texture;
      batch->info = eir_gfx_item_flag_default;
      if (editable)
      {
         batch->info |= eir_gfx_item_flag_editable;
      }
      if (visible)
      {
         batch->info |= eir_gfx_item_flag_visible;
      }
      if (use_screen_coord)
      {
         batch->info |= eir_gfx_item_flag_use_screen_coord;
      }
   }
   return batch;
}

// ---------------------------------------------------------------------------
static eir_gfx_sprite_t * eir_gfx_create_sprite_from_proxy(
   eir_gfx_sprite_batch_t * batch,
   const eir_gfx_sprite_proxy_t * sprite_proxy
   )
{
   eir_gfx_sprite_t * sprite = 0;

   if (batch && sprite_proxy)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->sprites, sprite)
   }
   if (sprite)
   {
      const eir_mth_vec2_t * position = &sprite_proxy->position;
      const eir_mth_vec2_t * size = &sprite_proxy->size;
      const eir_mth_vec2_t * uv_offset = &sprite_proxy->uv_offset;
      const eir_mth_vec2_t * uv_size = &sprite_proxy->uv_size;
      const eir_gfx_color_t * color = &sprite_proxy->color;

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
      batch->info |= eir_gfx_item_flag_modified;
   }
   else
   {
      EIR_KER_LOG_ERROR(
         "cannot add more sprite in batch. Capacity already reached"
         )
   }
   return sprite;
}

// ---------------------------------------------------------------------------
eir_gfx_sprite_proxy_t * eir_gfx_add_sprite_to_batch(
   eir_gfx_sprite_batch_t * batch,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_mth_vec2_t * uv_offset,
   const eir_mth_vec2_t * uv_size,
   const eir_gfx_color_t * color,
   bool visible
   )
{
   eir_gfx_sprite_proxy_t * sprite_proxy = 0;

   if (batch && position && size && uv_offset && uv_size && color)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->sprites_proxies, sprite_proxy);
   }
   if (sprite_proxy)
   {
      sprite_proxy->position.x = position->x;
      sprite_proxy->position.y = position->y;
      sprite_proxy->size.x = size->x;
      sprite_proxy->size.y = size->y;
      sprite_proxy->uv_offset.x = uv_offset->x;
      sprite_proxy->uv_offset.y = uv_offset->y;
      sprite_proxy->uv_size.x = uv_size->x;
      sprite_proxy->uv_size.y = uv_size->y;
      sprite_proxy->color.r = color->r;
      sprite_proxy->color.g = color->g;
      sprite_proxy->color.b = color->b;
      sprite_proxy->color.a = color->a;
      if (visible)
      {
         sprite_proxy->info |= eir_gfx_item_flag_visible;
      }
      sprite_proxy->info |= eir_gfx_item_flag_modified;
   }
   return sprite_proxy;
}

// ---------------------------------------------------------------------------
void eir_gfx_modify_sprite(
   eir_gfx_sprite_proxy_t * sprite_proxy,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_mth_vec2_t * uv_offset,
   const eir_mth_vec2_t * uv_size,
   const eir_gfx_color_t * color
   )
{
   if (sprite_proxy)
   {
      sprite_proxy->position.x = position->x;
      sprite_proxy->position.y = position->y;
      sprite_proxy->size.x = size->x;
      sprite_proxy->size.y = size->y;
      sprite_proxy->uv_offset.x = uv_offset->x;
      sprite_proxy->uv_offset.y = uv_offset->y;
      sprite_proxy->uv_size.x = uv_size->x;
      sprite_proxy->uv_size.y = uv_size->y;
      sprite_proxy->color.r = color->r;
      sprite_proxy->color.g = color->g;
      sprite_proxy->color.b = color->b;
      sprite_proxy->color.a = color->a;
      sprite_proxy->info |= eir_gfx_item_flag_modified;
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_sprite_visibility(
   eir_gfx_sprite_proxy_t * sprite_proxy,
   bool visible
   )
{
   if (sprite_proxy)
   {
      bool was_visible = sprite_proxy->info & eir_gfx_item_flag_visible;

      if (was_visible != visible)
      {
         if (was_visible)
         {
            sprite_proxy->info &= ~eir_gfx_item_flag_visible;
            if (
               sprite_proxy->buffer
               && ((eir_gfx_sprite_t *)sprite_proxy->buffer)->batch
               )
            {
               eir_gfx_sprite_batch_t * batch =
                  ((eir_gfx_sprite_t *)sprite_proxy->buffer)->batch;
               batch->info |= eir_gfx_item_flag_dirty;
            }
         }
         else
         {
            sprite_proxy->info |= eir_gfx_item_flag_visible;
         }
         sprite_proxy->info |= eir_gfx_item_flag_modified;
      }
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_sprite_batch(
   eir_gfx_env_t * env,
   eir_gfx_sprite_batch_t * batch
   )
{
   eir_mth_mat4_t id_mat;
   eir_mth_set_identity_mat4(&id_mat);

   if (batch && batch->info & eir_gfx_item_flag_visible)
   {
      if (!(batch->info & eir_gfx_item_flag_built))
      {
         eir_gfx_api_build_sprite_batch(  env, batch);
      }
      else if (batch->info & eir_gfx_item_flag_modified)
      {
         eir_gfx_api_bind_vertex_array(batch->vao);
         eir_gfx_api_bind_array_buffer(batch->vbo);
         eir_gfx_api_set_sprite_buffer_data(batch);
         eir_gfx_api_unbind_vertex_array();
         batch->info &= ~eir_gfx_item_flag_modified;
      }
      eir_gfx_api_bind_texture(batch->texture->id);
      eir_gfx_api_use_program(batch->program);
      eir_gfx_api_set_program_mat4(
         "pmat",
         batch->program,
         &env->projection
         );
      glUniform2f(
         glGetUniformLocation(batch->program, "atlasSize"),
         batch->texture->image->width,
         batch->texture->image->height
         );
      if (batch->info & eir_gfx_item_flag_use_screen_coord)
      {
         eir_gfx_api_set_program_mat4("vmat", batch->program, &id_mat);
      }
      else
      {
         eir_gfx_api_set_program_mat4("vmat", batch->program, &env->view);
      }
      eir_gfx_api_draw_sprite_batch(batch);
      eir_gfx_api_unuse_program();
   }
}

// ---------------------------------------------------------------------------
eir_gfx_rect_batch_t * eir_gfx_add_rect_batch_to_group(
   eir_gfx_group_t * group,
   size_t rect_count,
   bool editable,
   bool use_screen_coord,
   bool visible
   )
{
   eir_gfx_rect_batch_t * batch = 0;

   if (group && rect_count > 0)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(group->rect_batches, batch);
   }
   if (batch)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_rect_t,
         batch->rects,
         rect_count,
         eir_gfx_init_rect
         )
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_rect_proxy_t,
         batch->rects_proxies,
         rect_count,
         eir_gfx_init_rect_proxy
         )
      batch->info = eir_gfx_item_flag_default;
      if (editable)
      {
         batch->info |= eir_gfx_item_flag_editable;
      }
      if (visible)
      {
         batch->info |= eir_gfx_item_flag_visible;
      }
      if (use_screen_coord)
      {
         batch->info |= eir_gfx_item_flag_use_screen_coord;
      }
   }
   return batch;
}

// ---------------------------------------------------------------------------
static eir_gfx_rect_t * eir_gfx_create_rect_from_proxy(
   eir_gfx_rect_batch_t * batch,
   const eir_gfx_rect_proxy_t * rect_proxy
   )
{
   eir_gfx_rect_t * rect = 0;

   if (batch && rect_proxy)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->rects, rect)
   }
   if (rect)
   {
      const eir_mth_vec2_t * position = &rect_proxy->position;
      const eir_mth_vec2_t * size = &rect_proxy->size;
      const eir_gfx_color_t * color = &rect_proxy->color;

      rect->position.x = position->x;
      rect->position.y = position->y;
      rect->size.x = size->x;
      rect->size.y = size->y;
      rect->color.r = color->r;
      rect->color.g = color->g;
      rect->color.b = color->b;
      rect->color.a = color->a;
      rect->batch = batch;
      batch->info |= eir_gfx_item_flag_modified;
   }
   else
   {
      EIR_KER_LOG_ERROR(
         "cannot add more rect in batch. Capacity already reached"
         )
   }
   return rect;
}

// ---------------------------------------------------------------------------
eir_gfx_rect_proxy_t * eir_gfx_add_rect_to_batch(
   eir_gfx_rect_batch_t * batch,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_gfx_color_t * color,
   bool visible
   )
{
   eir_gfx_rect_proxy_t * rect_proxy = 0;

   if (batch && position && size && color)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->rects_proxies, rect_proxy);
   }
   if (rect_proxy)
   {
      rect_proxy->position.x = position->x;
      rect_proxy->position.y = position->y;
      rect_proxy->size.x = size->x;
      rect_proxy->size.y = size->y;
      rect_proxy->color.r = color->r;
      rect_proxy->color.g = color->g;
      rect_proxy->color.b = color->b;
      rect_proxy->color.a = color->a;
      if (visible)
      {
         rect_proxy->info |= eir_gfx_item_flag_visible;
      }
      rect_proxy->info |= eir_gfx_item_flag_modified;
   }
   return rect_proxy;
}

// ---------------------------------------------------------------------------
void eir_gfx_modify_rect(
   eir_gfx_rect_proxy_t * rect_proxy,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_gfx_color_t * color
   )
{
   if (rect_proxy)
   {
      rect_proxy->position.x = position->x;
      rect_proxy->position.y = position->y;
      rect_proxy->size.x = size->x;
      rect_proxy->size.y = size->y;
      rect_proxy->color.r = color->r;
      rect_proxy->color.g = color->g;
      rect_proxy->color.b = color->b;
      rect_proxy->color.a = color->a;
      rect_proxy->info |= eir_gfx_item_flag_modified;
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_rect_visibility(
   eir_gfx_rect_proxy_t * rect_proxy,
   bool visible
   )
{
   if (rect_proxy)
   {
      bool was_visible = rect_proxy->info & eir_gfx_item_flag_visible;

      if (was_visible != visible)
      {
         if (was_visible)
         {
            rect_proxy->info &= ~eir_gfx_item_flag_visible;
            if (
               rect_proxy->buffer
               && ((eir_gfx_rect_t *)rect_proxy->buffer)->batch
               )
            {
               eir_gfx_rect_batch_t * batch =
                  ((eir_gfx_rect_t *)rect_proxy->buffer)->batch;
               batch->info |= eir_gfx_item_flag_dirty;
            }
         }
         else
         {
            rect_proxy->info |= eir_gfx_item_flag_visible;
         }
         rect_proxy->info |= eir_gfx_item_flag_modified;
      }
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_rect_batch(
   eir_gfx_env_t * env,
   eir_gfx_rect_batch_t * batch
   )
{
   eir_mth_mat4_t id_mat;
   eir_mth_set_identity_mat4(&id_mat);

   if (batch && batch->info & eir_gfx_item_flag_visible)
   {
      if (!(batch->info & eir_gfx_item_flag_built))
      {
         eir_gfx_api_build_rect_batch(env, batch);
      }
      else if (batch->info & eir_gfx_item_flag_modified)
      {
         eir_gfx_api_bind_vertex_array(batch->vao);
         eir_gfx_api_bind_array_buffer(batch->vbo);
         eir_gfx_api_set_rect_buffer_data(batch);
         eir_gfx_api_unbind_vertex_array();
         batch->info &= ~eir_gfx_item_flag_modified;
      }
      eir_gfx_api_use_program(batch->program);
      eir_gfx_api_set_program_mat4(
         "rect_pmat",
         batch->program,
         &env->projection
         );
      if (batch->info & eir_gfx_item_flag_use_screen_coord)
      {
         eir_gfx_api_set_program_mat4("rect_vmat", batch->program, &id_mat);
      }
      else
      {
         eir_gfx_api_set_program_mat4(
            "rect_vmat",
            batch->program,
            &env->view
            );
      }
      eir_gfx_api_draw_rect_batch(batch);
      eir_gfx_api_unuse_program();
   }
}

// ---------------------------------------------------------------------------
eir_gfx_text_t * eir_gfx_add_text_to_group(
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
   eir_gfx_text_t * gfx_text = 0;

   if (group && text && position && color)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(group->texts, gfx_text)
   }
   if (gfx_text)
   {
      const float MAX_TEXTURE_WIDTH = 320;
      const float MAX_TEXTURE_HEIGHT = 320;
      const float MAX_TEXTURE_COL = 16;
      const float MAX_TEXTURE_ROW = 16;
      int text_len = strlen(text);

      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_sprite_t,
         gfx_text->sprites,
         text_len,
         eir_gfx_init_sprite
         );

      eir_mth_vec2_t uv_offset;
      eir_mth_vec2_t uv_size;
      eir_mth_vec2_t size;
      char c;
      float x_offset;
      float y_offset;

      size.x = 0.9f < font_size ?
         font_size : (MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL) / 4.0f;
      size.y = 0.9f < font_size ?
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
            sprite->batch = gfx_text;
         }
      }
      gfx_text->info |= eir_gfx_item_flag_modified;
      if (editable)
      {
         gfx_text->info |= eir_gfx_item_flag_editable;
      }
      if (visible)
      {
         gfx_text->info |= eir_gfx_item_flag_visible;
      }
      if (use_screen_coord)
      {
         gfx_text->info |= eir_gfx_item_flag_use_screen_coord;
      }
   }
   return gfx_text;
}

// ---------------------------------------------------------------------------
void eir_gfx_modify_text(
   eir_gfx_text_t * gfx_text,
   const char * text,
   const eir_mth_vec2_t * position,
   float font_size,
   const eir_gfx_color_t * color
   )
{
   if (gfx_text && gfx_text->info & eir_gfx_item_flag_editable)
   {
      EIR_KER_FREE_ARRAY_BIS(
         gfx_text->sprites,
         eir_gfx_release_sprite
         )

      const float MAX_TEXTURE_WIDTH = 320;
      const float MAX_TEXTURE_HEIGHT = 320;
      const float MAX_TEXTURE_COL = 16;
      const float MAX_TEXTURE_ROW = 16;
      int text_len = strlen(text);

      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gfx_sprite_t,
         gfx_text->sprites,
         text_len,
         eir_gfx_init_sprite
         )

      eir_mth_vec2_t uv_offset;
      eir_mth_vec2_t uv_size;
      eir_mth_vec2_t size;
      char c;
      float x_offset;
      float y_offset;

      size.x = 0.9f < font_size ?
         font_size : (MAX_TEXTURE_WIDTH / MAX_TEXTURE_COL) / 4.0f;
      size.y = 0.9f < font_size ?
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
            sprite->batch = gfx_text;
         }
      }
      gfx_text->info |= eir_gfx_item_flag_modified;
   }
}

// ---------------------------------------------------------------------------
void eir_gfx_set_text_visibility(
   eir_gfx_text_t * gfx_text,
   bool visible
   )
{
   if (gfx_text)
   {
      if ((gfx_text->info & eir_gfx_item_flag_visible) && !visible)
      {
         gfx_text->info &= ~eir_gfx_item_flag_visible;
      }
      else if (!(gfx_text->info & eir_gfx_item_flag_visible) && visible)
      {
         gfx_text->info |= eir_gfx_item_flag_visible;
      }
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_text(
   eir_gfx_env_t * env,
   eir_gfx_text_t * text
   )
{
   eir_mth_mat4_t id_mat;
   eir_mth_set_identity_mat4(&id_mat);

   if (text && text->info & eir_gfx_item_flag_visible)
   {
      if (!(text->info & eir_gfx_item_flag_built))
      {
         eir_gfx_api_build_text_batch(env, text);
      }
      else if (text->info & eir_gfx_item_flag_modified)
      {
         eir_gfx_api_bind_vertex_array(text->vao);
         eir_gfx_api_bind_array_buffer(text->vbo);
         eir_gfx_api_set_sprite_buffer_data(text);
         eir_gfx_api_unbind_vertex_array();
         text->info &= ~eir_gfx_item_flag_modified;
      }
      eir_gfx_api_bind_texture(text->texture->id);
      eir_gfx_api_use_program(text->program);
      eir_gfx_api_set_program_mat4(
         "pmat",
         text->program,
         &env->projection
         );
      glUniform2f(
         glGetUniformLocation(text->program, "atlasSize"),
         text->texture->image->width,
         text->texture->image->height
         );
      if (text->info & eir_gfx_item_flag_use_screen_coord)
      {
         eir_gfx_api_set_program_mat4("vmat", text->program, &id_mat);
      }
      else
      {
         eir_gfx_api_set_program_mat4("vmat", text->program, &env->view);
      }
      eir_gfx_api_draw_sprite_batch(text);
      eir_gfx_api_unuse_program();
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_update_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   if (batch && batch->info & eir_gfx_item_flag_visible)
   {
      if (batch->info & eir_gfx_item_flag_dirty)
      {
         EIR_KER_RELEASE_ARRAY_BIS(
            batch->sprites,
            eir_gfx_release_sprite
            );
         batch->info &= ~eir_gfx_item_flag_dirty;
      }
      for (int index = 0; index < batch->sprites_proxies.used; ++index)
      {
         eir_gfx_sprite_proxy_t * proxy = &batch->sprites_proxies.data[index];
         if (
            (proxy->info & eir_gfx_item_flag_visible)
            && !(proxy->info & eir_gfx_item_flag_added)
            )
         {
            proxy->buffer = eir_gfx_create_sprite_from_proxy(batch, proxy);
            proxy->info |= eir_gfx_item_flag_added;
            batch->info |= eir_gfx_item_flag_modified;
         }
         if (
            (proxy->info & eir_gfx_item_flag_visible)
            && (proxy->info & eir_gfx_item_flag_modified)
            && proxy->buffer
            )
         {
            ((eir_gfx_sprite_t * )proxy->buffer)->position.x = proxy->position.x;
            ((eir_gfx_sprite_t * )proxy->buffer)->position.y = proxy->position.y;
            ((eir_gfx_sprite_t * )proxy->buffer)->size.x = proxy->size.x;
            ((eir_gfx_sprite_t * )proxy->buffer)->size.y = proxy->size.y;
            ((eir_gfx_sprite_t * )proxy->buffer)->uv_offset.x = proxy->uv_offset.x;
            ((eir_gfx_sprite_t * )proxy->buffer)->uv_offset.y = proxy->uv_offset.y;
            ((eir_gfx_sprite_t * )proxy->buffer)->uv_size.x = proxy->uv_size.x;
            ((eir_gfx_sprite_t * )proxy->buffer)->uv_size.y = proxy->uv_size.y;
            ((eir_gfx_sprite_t * )proxy->buffer)->color.r = proxy->color.r;
            ((eir_gfx_sprite_t * )proxy->buffer)->color.g = proxy->color.g;
            ((eir_gfx_sprite_t * )proxy->buffer)->color.b = proxy->color.b;
            ((eir_gfx_sprite_t * )proxy->buffer)->color.a = proxy->color.a;
            proxy->info &= ~eir_gfx_item_flag_modified;
            batch->info |= eir_gfx_item_flag_modified;
         }
      }
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_update_rect_batch(eir_gfx_rect_batch_t * batch)
{
   if (batch && batch->info & eir_gfx_item_flag_visible)
   {
      if (batch->info & eir_gfx_item_flag_dirty)
      {
         EIR_KER_RELEASE_ARRAY_BIS(
            batch->rects,
            eir_gfx_release_rect
            );
         batch->info &= ~eir_gfx_item_flag_dirty;
      }
      for (int index = 0; index < batch->rects_proxies.used; ++index)
      {
         eir_gfx_rect_proxy_t * proxy = &batch->rects_proxies.data[index];
         if (
            (proxy->info & eir_gfx_item_flag_visible)
            && !(proxy->info & eir_gfx_item_flag_added)
            )
         {
            proxy->buffer = eir_gfx_create_rect_from_proxy(batch, proxy);
            proxy->info |= eir_gfx_item_flag_added;
            batch->info |= eir_gfx_item_flag_modified;
         }
         if (
            (proxy->info & eir_gfx_item_flag_visible)
            && (proxy->info & eir_gfx_item_flag_modified)
            && proxy->buffer
            )
         {
            ((eir_gfx_rect_t * )proxy->buffer)->position.x = proxy->position.x;
            ((eir_gfx_rect_t * )proxy->buffer)->position.y = proxy->position.y;
            ((eir_gfx_rect_t * )proxy->buffer)->size.x = proxy->size.x;
            ((eir_gfx_rect_t * )proxy->buffer)->size.y = proxy->size.y;
            ((eir_gfx_rect_t * )proxy->buffer)->color.r = proxy->color.r;
            ((eir_gfx_rect_t * )proxy->buffer)->color.g = proxy->color.g;
            ((eir_gfx_rect_t * )proxy->buffer)->color.b = proxy->color.b;
            ((eir_gfx_rect_t * )proxy->buffer)->color.a = proxy->color.a;
            proxy->info &= ~eir_gfx_item_flag_modified;
            batch->info |= eir_gfx_item_flag_modified;
         }
      }
   }
}

// ---------------------------------------------------------------------------
static void eir_gfx_render_group(eir_gfx_env_t * env, eir_gfx_group_t * group)
{
   if (env && group && group->visible)
   {
      for (int index = 0; index < group->sprite_batches.used; ++index)
      {
         eir_gfx_update_sprite_batch(&group->sprite_batches.data[index]);
         eir_gfx_render_sprite_batch(env, &group->sprite_batches.data[index]);
      }
      for (int index = 0; index < group->rect_batches.used; ++index)
      {
         eir_gfx_update_rect_batch(&group->rect_batches.data[index]);
         eir_gfx_render_rect_batch(env, &group->rect_batches.data[index]);
      }
      for (int index = 0; index < group->texts.used; ++index)
      {
         eir_gfx_render_text(env, &group->texts.data[index]);
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
}

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

