#include "eir_gfx_func.h"
#include "eir_gfx_api_func.h"
#include "../maths/eir_mth_func.h"

static void eir_gfx_debug_log_sprite(eir_gfx_sprite_t * sprite)
{
   EIR_KER_LOG_MESSAGE("---------------------------");
   EIR_KER_LOG_MESSAGE("sprite info:");
   EIR_KER_LOG_MESSAGE("position = (%f, %f)", sprite->position.x, sprite->position.y);
   EIR_KER_LOG_MESSAGE("size = (%f, %f)", sprite->size.x, sprite->size.y);
   EIR_KER_LOG_MESSAGE("uv_offset = (%f, %f)", sprite->uv_offset.x, sprite->uv_offset.y);
   EIR_KER_LOG_MESSAGE("uv_size = (%f, %f)", sprite->uv_size.x, sprite->uv_size.y);
   EIR_KER_LOG_MESSAGE(
      "color = (%f, %f, %f, %f)",
      sprite->color.r,
      sprite->color.g,
      sprite->color.b,
      sprite->color.a
      );
   EIR_KER_LOG_MESSAGE("---------------------------");
}

static void eir_gfx_debug_log_rect(eir_gfx_rect_t * rect)
{
   EIR_KER_LOG_MESSAGE("---------------------------");
   EIR_KER_LOG_MESSAGE("rect info:");
   EIR_KER_LOG_MESSAGE("position = (%f, %f)", rect->position.x, rect->position.y);
   EIR_KER_LOG_MESSAGE("size = (%f, %f)", rect->size.x, rect->size.y);
   EIR_KER_LOG_MESSAGE(
      "color = (%f, %f, %f, %f)",
      rect->color.r,
      rect->color.g,
      rect->color.b,
      rect->color.a
      );
   EIR_KER_LOG_MESSAGE("---------------------------");
}

static void eir_gfx_init_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   if (batch)
   {
      EIR_KER_INIT_ARRAY(batch->sprites);
      batch->vbo = EIR_GFX_INVALID_VBO_HANDLE;
      batch->vao = EIR_GFX_INVALID_VAO_HANDLE;
      batch->texture = 0;
      batch->program = EIR_GFX_INVALID_PROGRAM_HANDLE;
      batch->built = false;
      batch->modified = false;
   }
}

static void eir_gfx_release_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   if (batch)
   {
      eir_gfx_api_release_sprite_batch(batch);
      EIR_KER_FREE_ARRAY(batch->sprites);
      eir_gfx_init_sprite_batch(batch);
   }
}

static void eir_gfx_init_rect_batch(eir_gfx_rect_batch_t * batch)
{
   if (batch)
   {
      EIR_KER_INIT_ARRAY(batch->rects);
      batch->vbo = EIR_GFX_INVALID_VBO_HANDLE;
      batch->vao = EIR_GFX_INVALID_VAO_HANDLE;
      batch->program = EIR_GFX_INVALID_PROGRAM_HANDLE;
      batch->built = false;
      batch->modified = false;
   }
}

static void eir_gfx_release_rect_batch(eir_gfx_rect_batch_t * batch)
{
   if (batch)
   {
      eir_gfx_api_release_rect_batch(batch);
      EIR_KER_FREE_ARRAY(batch->rects);
      eir_gfx_init_rect_batch(batch);
   }
}

static void eir_gfx_init_image(eir_gfx_image_t * image)
{
   if (image)
   {
      image->pixels = 0;
      image->width = 0;
      image->height = 0;
   }
}

static void eir_gfx_release_image(eir_gfx_image_t * image)
{
   if (image)
   {
      EIR_SYS_FREE(image->pixels);
      eir_gfx_init_image(image);
   }
}

static void eir_gfx_init_sprite_ref(eir_gfx_sprite_ref_t * sprite_ref)
{
   if (sprite_ref)
   {
      sprite_ref->uv_offset.x = 0.0f;
      sprite_ref->uv_offset.y = 0.0f;
      sprite_ref->uv_size.x = 0.0f;
      sprite_ref->uv_size.y = 0.0f;
      sprite_ref->texture = 0;
   }
}

static void eir_gfx_release_sprite_ref(eir_gfx_sprite_ref_t * sprite_ref)
{
   eir_gfx_init_sprite_ref(sprite_ref);
}

static void eir_gfx_init_texture(eir_gfx_texture_t * texture)
{
   if (texture)
   {
      texture->id = EIR_GFX_INVALID_TEXTURE_HANDLE;
      texture->image = 0;
   }
}

static void eir_gfx_release_texture(eir_gfx_texture_t * texture)
{
   if (texture)
   {
      eir_gfx_api_destroy_texture(texture->id);
      eir_gfx_init_texture(texture);
   }
}

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
   }
}

static void eir_gfx_release_rect(eir_gfx_rect_t * rect)
{
   eir_gfx_init_rect(rect);
}

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
      EIR_KER_INIT_ARRAY(gfx_env->sprite_batches);
      EIR_KER_INIT_ARRAY(gfx_env->text_batches);
      EIR_KER_INIT_ARRAY(gfx_env->images);
      EIR_KER_INIT_ARRAY(gfx_env->sprites_ref);
      EIR_KER_INIT_ARRAY(gfx_env->textures);
      EIR_KER_INIT_ARRAY(gfx_env->rect_batch.rects);
   }
}

void eir_gfx_release_env(eir_gfx_env_t * gfx_env)
{
   EIR_KER_LOG_MESSAGE("release graphics env");
   if (gfx_env)
   {
      eir_gfx_api_destroy_texture(gfx_env->text_texture.id);
      EIR_SYS_FREE(gfx_env->text_image.pixels);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->sprite_batches, eir_gfx_release_sprite_batch);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->text_batches, eir_gfx_release_sprite_batch);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->sprites_ref, eir_gfx_release_sprite_ref);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->images, eir_gfx_release_image);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->textures, eir_gfx_release_texture);
      EIR_KER_FREE_ARRAY_BIS(gfx_env->rect_batch.rects, eir_gfx_release_rect);
   }
}

void eir_gfx_set_sprite_batch_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gfx_sprite_batch_t,
	 gfx_env->sprite_batches,
	 max_capacity,
	 eir_gfx_init_sprite_batch
	 );
   }
}

void eir_gfx_set_text_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gfx_sprite_batch_t,
	 gfx_env->text_batches,
	 max_capacity,
	 eir_gfx_init_sprite_batch
	 );
   }
}

void eir_gfx_set_rect_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      gfx_env->rect_batch.built = false;
      gfx_env->rect_batch.modified = false;
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gfx_rect_t,
	 gfx_env->rect_batch.rects,
	 max_capacity,
	 eir_gfx_init_rect
	 );
   }
}

static eir_gfx_sprite_batch_t * eir_gfx_create_sprite_batch(eir_gfx_env_t * gfx_env, int max_capacity)
{
   eir_gfx_sprite_batch_t * batch = 0;

   if (gfx_env && max_capacity > 0)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(gfx_env->sprite_batches, batch);
   }
   if (batch)
   {
      EIR_KER_ALLOCATE_ARRAY(eir_gfx_sprite_t, batch->sprites, max_capacity);
      batch->built = false;
      batch->modified = false;
   }
   return batch;
}

static eir_gfx_sprite_t * eir_gfx_add_sprite_to_batch(
   eir_gfx_sprite_batch_t * batch,
   const eir_mth_vec2_t * position,
   const eir_mth_vec2_t * size,
   const eir_mth_vec2_t * uv_offset,
   const eir_mth_vec2_t * uv_size,
   const eir_gfx_color_t * color
   )
{
   eir_gfx_sprite_t * sprite = 0;

   if (batch)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->sprites, sprite);
   }
   if (sprite && position && size && uv_offset && uv_size && color)
   {
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
      eir_gfx_debug_log_sprite(sprite);
      batch->modified = true;
   }
   return sprite;
}

eir_handle_t eir_gfx_add_text(
   eir_gfx_env_t * gfx_env,
   const char * text,
   eir_mth_vec2_t * position,
   float font_size,
   eir_gfx_color_t * color
   )
{
   if (!gfx_env || !text || !position || !color)
   {
      return EIR_INVALID_HANDLE;
   }

   // TODO: move this anywhere else !
   const float MAX_TEXTURE_WIDTH = 320;
   const float MAX_TEXTURE_HEIGHT = 320;
   const float MAX_TEXTURE_COL = 16;
   const float MAX_TEXTURE_ROW = 16;

   int text_len = strlen(text);
   eir_handle_t batch_handle = EIR_INVALID_HANDLE;
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
      }
   }
   return batch_handle;
}

static eir_gfx_rect_t * eir_gfx_add_rect(
   eir_gfx_env_t * gfx_env,
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_gfx_color_t * color
   )
{
   eir_gfx_rect_t * rect = 0;
   eir_handle_t handle = EIR_INVALID_HANDLE;

   if (gfx_env && size && position && color)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->rect_batch.rects, rect, handle);
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
      eir_gfx_debug_log_rect(rect);
      gfx_env->rect_batch.modified = true;
   }
   return rect;
}

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

void eir_gfx_render_all_batches(eir_gfx_env_t * gfx_env)
{
   if (!gfx_env)
   {
      return;
   }

   eir_gfx_sprite_batch_t * batch = 0;

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
	    glBindVertexArray(batch->vao);
	    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	    eir_gfx_api_set_sprite_buffer_data(batch);
	    glBindVertexArray(0);
	 }
	 glBindTexture(GL_TEXTURE_2D, batch->texture->id); // TODO: put in api func file !
	 glUseProgram(batch->program); // TODO: put in the api func file !
	 glUniformMatrix4fv(
	    glGetUniformLocation(batch->program, "pmat"),
	    1,
	    GL_FALSE,
	    &gfx_env->projection.values[0][0]
	    );
	 eir_gfx_api_draw_sprite_batch(batch);
	 glUseProgram(0); // TODO: put in api func file !
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
	    glBindVertexArray(batch->vao);
	    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	    eir_gfx_api_set_sprite_buffer_data(batch);
	    glBindVertexArray(0);
	 }
	 glBindTexture(GL_TEXTURE_2D, batch->texture->id); // TODO: put in api func file !
	 glUseProgram(batch->program); // TODO: put in the api func file !
	 glUniformMatrix4fv(
	    glGetUniformLocation(batch->program, "pmat"),
	    1,
	    GL_FALSE,
	    &gfx_env->projection.values[0][0]
	    );
	 eir_gfx_api_draw_sprite_batch(batch);
	 glUseProgram(0); // TODO: put in api func file !
      }
   }
   if (0 < gfx_env->rect_batch.rects.used)
   {
      if (!gfx_env->rect_batch.built)
      {
	 eir_gfx_api_build_rect_batch(gfx_env, &gfx_env->rect_batch); // TODO: to declare/implement
      }
      else if (gfx_env->rect_batch.modified)
      {
	 glBindVertexArray(gfx_env->rect_batch.vao);
	 glBindBuffer(GL_ARRAY_BUFFER, gfx_env->rect_batch.vbo);
	 eir_gfx_api_set_rect_buffer_data(&gfx_env->rect_batch); // TODO: to declare/implement
	 glBindVertexArray(0);
      }
      glUseProgram(gfx_env->rect_batch.program); // TODO: put in the api func file !
      glUniformMatrix4fv(
	    glGetUniformLocation(gfx_env->rect_batch.program, "rect_pmat"),
	    1,
	    GL_FALSE,
	    &gfx_env->projection.values[0][0]
	    );
      eir_gfx_api_draw_rect_batch(&gfx_env->rect_batch); // TODO: to declare/implement
      glUseProgram(0); // TODO: put in api func file !
   }
   eir_gfx_api_check_for_error();
}

void eir_gfx_update_text(
   eir_gfx_env_t * gfx_env,
   eir_handle_t text_handle,
   const char * text
   )
{
   if (!gfx_env || EIR_INVALID_HANDLE == text_handle || !text)
   {
      return;
   }

   eir_gfx_sprite_batch_t * batch = 0;
   EIR_KER_GET_ARRAY_ITEM(gfx_env->text_batches, text_handle, batch);

   if (!batch)
   {
      return;
   }
   
   eir_mth_vec2_t position;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_mth_vec2_t size;
   eir_gfx_color_t color;

   // TODO get attributes values from first text sprite element

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

void eir_gfx_set_max_image_count(eir_env_t * env, size_t max_count)
{
   eir_gfx_env_t * gfx_env = 0;

   if (env)
   {
      gfx_env = &(((eir_ker_env_t *)env->private)->gfx_env);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gfx_image_t, gfx_env->images, max_count, eir_gfx_init_image);
   }
}

eir_handle_t eir_gfx_load_image(eir_env_t * env, const char * img_filename, bool must_invert_img)
{
   eir_gfx_env_t * gfx_env = 0;

   if (env)
   {
      gfx_env = &(((eir_ker_env_t *)env->private)->gfx_env);
   }

   eir_handle_t image_handle = EIR_INVALID_HANDLE;
   eir_gfx_image_t * image = 0;

   if (gfx_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->images, image, image_handle);
   }
   if (image)
   {
      bool result = eir_gfx_api_load_image(img_filename, must_invert_img, image);

      if (!result)
      {
	 EIR_KER_LOG_ERROR("cannot load image from file %d", img_filename);
	 eir_gfx_release_image(image);
	 EIR_KER_FREE_ARRAY_LAST_RESERVED_SLOT(gfx_env->images);
	 image_handle = EIR_INVALID_HANDLE;
      }
      else
      {
	 EIR_KER_LOG_MESSAGE("%s has been loaded", img_filename);
      }
   }
   return image_handle;
}

void eir_gfx_set_max_texture_count(eir_env_t * env, size_t max_count)
{
   eir_gfx_env_t * gfx_env = 0;

   if (env)
   {
      gfx_env = &(((eir_ker_env_t *)env->private)->gfx_env);
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gfx_texture_t,
	 gfx_env->textures,
	 max_count,
	 eir_gfx_init_texture
	 );
   }
}

eir_handle_t eir_gfx_create_texture(eir_env_t * env, eir_handle_t img_handle)
{
   eir_gfx_env_t * gfx_env = 0;

   if (env)
   {
      gfx_env = &(((eir_ker_env_t *)env->private)->gfx_env);
   }

   eir_gfx_image_t * image = 0;
   eir_gfx_texture_t * texture = 0;
   eir_handle_t texture_handle = EIR_INVALID_HANDLE;

   if (gfx_env)
   {
      EIR_KER_GET_ARRAY_ITEM(gfx_env->images, img_handle, image);
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->textures, texture, texture_handle);
   }
   if (texture)
   {
      if (image)
      {
	 texture->id = eir_gfx_api_create_texture(image);
	 texture->image = image;
      }
      if (!image || texture->id == EIR_GFX_INVALID_TEXTURE_HANDLE)
      {
	 if (!image)
	 {
	    EIR_KER_LOG_ERROR("cannot find image from handle %d", img_handle);
	 }
	 else if (texture->id == EIR_GFX_INVALID_TEXTURE_HANDLE)
	 {
	    EIR_KER_LOG_ERROR("cannot create texture from image handle %d", img_handle);
	 }
	 EIR_KER_LOG_ERROR("create texture from image %d has failed: release texture slot", img_handle);
	 eir_gfx_release_texture(texture);
	 EIR_KER_FREE_ARRAY_LAST_RESERVED_SLOT(gfx_env->textures);
	 texture_handle = EIR_INVALID_HANDLE;
      }
   }
   return texture_handle;
}

void eir_gfx_set_max_sprite_ref_count(eir_env_t * env, size_t max_count)
{
   if (env)
   {
      eir_gfx_env_t * gfx_env = &(((eir_ker_env_t *)env->private)->gfx_env);

      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gfx_sprite_ref_t,
	 gfx_env->sprites_ref,
	 max_count,
	 eir_gfx_init_sprite_ref
	 );
   } 
}

eir_handle_t eir_gfx_create_sprite_ref(
   eir_env_t * env,
   eir_handle_t texture_handle,
   int img_x_offset,
   int img_y_offset,
   int img_width_offset,
   int img_height_offset
   )
{
   eir_handle_t sprite_ref_handle = EIR_INVALID_HANDLE;
   eir_gfx_sprite_ref_t * sprite_ref = 0;
   eir_gfx_texture_t * texture = 0;
   eir_gfx_env_t * gfx_env = 0;
      
   if (env)
   {
      gfx_env = &(((eir_ker_env_t *)env->private)->gfx_env);
   }
   if (gfx_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->sprites_ref, sprite_ref, sprite_ref_handle);
   }
   if (sprite_ref)
   {
      EIR_KER_LOG_MESSAGE("create sprite ref with texture %d", texture_handle)
      sprite_ref->uv_offset.x = (float)img_x_offset;
      sprite_ref->uv_offset.y = (float)img_y_offset;
      sprite_ref->uv_size.x = (float)img_width_offset;
      sprite_ref->uv_size.y = (float)img_height_offset;
      EIR_KER_GET_ARRAY_ITEM(gfx_env->textures, texture_handle, texture);
      if (!texture)
      {
	 EIR_KER_LOG_ERROR("cannot find texture %d in textures array", texture_handle);
      }
      sprite_ref->texture = texture;
   }
   return sprite_ref_handle;
}

void eir_gfx_generate_all_batches(eir_gfx_env_t * gfx_env, const eir_gme_world_t * world)
{
   EIR_KER_LOG_MESSAGE("generate all batches from specified world");
   if (gfx_env && world)
   {
      size_t needed_batch_count = 0;
      eir_gme_entity_t * entity = 0;

      EIR_KER_LOG_MESSAGE("%d entities found", world->entities.used);

      for (int i = 0; i < world->entities.used; ++i)
      {
	 EIR_KER_LOG_MESSAGE("entity %d flags = %d", i, world->entities.data[i]);
	 if ((world->entities.data[i] & eir_gme_component_type_sprite) == eir_gme_component_type_sprite)
	 {
	    EIR_KER_LOG_MESSAGE("entity %d has sprite", i);

	    int j;

	    for (j = 0; j < i; ++j)
	    {
	       if (world->sprite_ref_handles.data[i] == world->sprite_ref_handles.data[j])
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

      for (int entity_index = 0; entity_index < world->entities.used; ++entity_index)
      {
	 EIR_KER_GET_ARRAY_ITEM(world->entities, entity_index, entity);
	 if (entity && ((*entity) & eir_gme_component_type_sprite))
	 {
	    int batch_item_count = 1;
	    eir_handle_t * sprite_ref_handle_ptr = 0;
	    eir_handle_t sprite_ref_handle = EIR_INVALID_HANDLE;

	    EIR_KER_GET_ARRAY_ITEM(world->sprite_ref_handles, entity_index, sprite_ref_handle_ptr);

	    if (sprite_ref_handle_ptr)
	    {
	       sprite_ref_handle = *sprite_ref_handle_ptr;
	    }
	    if (sprite_ref_handle != EIR_INVALID_HANDLE)
	    {
	       eir_gfx_sprite_ref_t * sprite_ref = 0;
	       
	       EIR_KER_GET_ARRAY_ITEM(
		  gfx_env->sprites_ref,
		  sprite_ref_handle,
		  sprite_ref
		  );

	       if (!sprite_ref)
	       {
		  EIR_KER_LOG_ERROR("cannot get sprite ref %d", sprite_ref_handle);
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
		  eir_handle_t * sibling_sprite_ref_handle_ptr = 0;
		  EIR_KER_GET_ARRAY_ITEM(
		     world->sprite_ref_handles,
		     sibling_entity_index,
		     sibling_sprite_ref_handle_ptr
		     );
		  
		  while (sibling_sprite_ref_handle_ptr)
		  {
		     if (*sibling_sprite_ref_handle_ptr == sprite_ref_handle)
		     {
			++batch_item_count;
		     }
		     EIR_KER_GET_ARRAY_ITEM(
			world->sprite_ref_handles,
			++sibling_entity_index,
			sibling_sprite_ref_handle_ptr
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

      eir_gme_camera_component_t * camera = world->curr_camera;

      if (camera)
      {
	 eir_gfx_color_t color;

	 color.r = 1.0f;
	 color.g = 0.5f;
	 color.b = 0.0f;
	 color.a = 0.3f;
	 camera->cam_win_rect =  eir_gfx_add_rect(
	    gfx_env,
	    &camera->cam_win_aabb.position,
	    &camera->cam_win_aabb.size,
	    &color
	    );

	 EIR_KER_LOG_MESSAGE(
	    "camera found. add window debug rect (%f; %f; %f; %f)",
	    camera->cam_win_aabb.position.x,
	    camera->cam_win_aabb.position.y,
	    camera->cam_win_aabb.size.x,
	    camera->cam_win_aabb.size.y
	    );
      }

      for (int entity_index = 0; entity_index < world->entities.used; ++entity_index)
      {
	 EIR_KER_GET_ARRAY_ITEM(world->entities, entity_index, entity);

	 if (entity && ((*entity) & eir_gme_component_type_aabb))
	 {
	    eir_gme_aabb_component_t * aabb = 0;

	    EIR_KER_GET_ARRAY_ITEM(
		  world->aabbs,
		  entity_index,
		  aabb
		  );

	    if (aabb)
	    {
	       eir_gfx_color_t color;

	       color.r = 1.0f;
	       color.g = 1.0f;
	       color.b = 0.0f;
	       color.a = 0.3f;
	       aabb->curr_rect =  eir_gfx_add_rect(
		  gfx_env,
		  &aabb->initial.position,
		  &aabb->initial.size,
		  &color
		  );

	       EIR_KER_LOG_MESSAGE(
		  "entity has aabb. add debug rect (%f; %f; %f; %f)",
		  aabb->initial.position.x,
		  aabb->initial.position.y,
		  aabb->initial.size.x,
		  aabb->initial.size.y
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
