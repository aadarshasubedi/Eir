#include "eir_gfx_func.h"
#include "eir_gfx_api_func.h"

static void eir_gfx_debug_log_sprite(eir_gfx_sprite_t * sprite)
{
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
   EIR_KER_LOG_MESSAGE("%s", "sprite info:");
   EIR_KER_LOG_MESSAGE("position = (%f, %f)", sprite->position.x, sprite->position.y);
   EIR_KER_LOG_MESSAGE("size = (%f, %f)", sprite->size.x, sprite->size.y);
   EIR_KER_LOG_MESSAGE("uv_offset = (%f, %f)", sprite->uv_offset.x, sprite->uv_offset.y);
   EIR_KER_LOG_MESSAGE("uv_size = (%f, %f)", sprite->uv_size.x, sprite->uv_size.y);
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
}

void eir_gfx_set_batch_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      EIR_KER_INIT_ARRAY(eir_gfx_sprite_batch_t, gfx_env->sprite_batches, max_capacity);
   }
}

void eir_gfx_set_text_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      EIR_KER_INIT_ARRAY(eir_gfx_sprite_batch_t, gfx_env->text_batches, max_capacity);
   }
}

void eir_gfx_set_line_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      gfx_env->line_batch.built = false;
      gfx_env->line_batch.modified = false;
      gfx_env->line_batch.primitive_type = eir_gfx_primitive_type_lines;
      EIR_KER_INIT_ARRAY(eir_gfx_vertex_t, gfx_env->line_batch.vertices, max_capacity * 2);
   }
}

void eir_gfx_set_quad_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      gfx_env->quad_batch.built = false;
      gfx_env->quad_batch.modified = false;
      gfx_env->quad_batch.primitive_type = eir_gfx_primitive_type_quads;
      EIR_KER_INIT_ARRAY(eir_gfx_vertex_t, gfx_env->quad_batch.vertices, max_capacity * 4);
   }
}

eir_handle_t eir_gfx_create_sprite_batch(eir_gfx_env_t * gfx_env, int max_capacity)
{
   eir_handle_t batch_handle = EIR_INVALID_HANDLE;
   eir_gfx_sprite_batch_t * batch = 0;

   if (gfx_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->sprite_batches, batch, batch_handle);
   }
   if (batch)
   {
      EIR_KER_INIT_ARRAY(eir_gfx_sprite_t, batch->sprites, max_capacity);
      batch->built = false;
      batch->modified = false;
   }
   return batch_handle;
}

eir_handle_t eir_gfx_add_sprite(
   eir_gfx_env_t * gfx_env,
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   eir_gfx_color_t * color,
   eir_handle_t batch_handle
   )
{
   eir_handle_t sprite_handle = EIR_INVALID_HANDLE;
   eir_gfx_sprite_batch_t * batch = 0;
   eir_gfx_sprite_t * sprite = 0;

   EIR_KER_GET_ARRAY_ITEM(gfx_env->sprite_batches, batch_handle, batch);

   if (batch)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(batch->sprites, sprite, sprite_handle);
   }
   if (sprite)
   {
      // TODO: check all pointers, set default value for null pointer
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
   return sprite_handle;
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
      EIR_KER_INIT_ARRAY(eir_gfx_sprite_t, batch->sprites, text_len);
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
      y_offset = MAX_TEXTURE_ROW - ((int)c / (int)MAX_TEXTURE_ROW) - 1;  
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

eir_handle_t eir_gfx_add_line(
   eir_gfx_env_t * gfx_env,
   eir_mth_vec2_t * a,
   eir_mth_vec2_t * b,
   eir_gfx_color_t * a_color,
   eir_gfx_color_t * b_color
   )
{
   eir_gfx_vertex_t * a_vertex = 0;
   eir_gfx_vertex_t * b_vertex = 0;
   eir_handle_t a_handle = EIR_INVALID_HANDLE;
   eir_handle_t b_handle = EIR_INVALID_HANDLE;

   if (gfx_env && a && b && a_color && b_color)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->line_batch.vertices, a_vertex, a_handle);
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->line_batch.vertices, b_vertex, b_handle);
   }
   if (a_vertex && b_vertex)
   {
      a_vertex->position.x = a->x;
      a_vertex->position.y = a->y;
      a_vertex->position.z = 0.0f;
      a_vertex->color.r = a_color->r;
      a_vertex->color.g = a_color->g;
      a_vertex->color.b = a_color->b;
      a_vertex->color.a = a_color->a;

      b_vertex->position.x = b->x;
      b_vertex->position.y = b->y;
      b_vertex->position.z = 0.0f;
      b_vertex->color.r = b_color->r;
      b_vertex->color.g = b_color->g;
      b_vertex->color.b = b_color->b;
      b_vertex->color.a = b_color->a;
   }
   return a_handle;
}

eir_handle_t eir_gfx_add_quad(
   eir_gfx_env_t * gfx_env,
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_gfx_color_t * color
   )
{
   eir_gfx_vertex_t * a = 0;
   eir_gfx_vertex_t * b = 0;
   eir_gfx_vertex_t * c = 0;
   eir_gfx_vertex_t * d = 0;
   eir_handle_t a_handle = EIR_INVALID_HANDLE;
   eir_handle_t b_handle = EIR_INVALID_HANDLE;
   eir_handle_t c_handle = EIR_INVALID_HANDLE;
   eir_handle_t d_handle = EIR_INVALID_HANDLE;

   if (gfx_env && position && size && color)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->quad_batch.vertices, a, a_handle);
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->quad_batch.vertices, b, b_handle);
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->quad_batch.vertices, c, c_handle);
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gfx_env->quad_batch.vertices, d, d_handle);
   }
   if (a && b && c && d)
   {
      a->position.x = position->x + size->x;
      a->position.y = position->y + size->y;
      a->position.z = 0.0f;
      a->color.r = color->r;
      a->color.g = color->g;
      a->color.b = color->b;
      a->color.a = color->a;

      b->position.x = position->x;
      b->position.y = position->y + size->y;
      b->position.z = 0.0f;
      b->color.r = color->r;
      b->color.g = color->g;
      b->color.b = color->b;
      b->color.a = color->a;

      c->position.x = position->x + size->x;
      c->position.y = position->y;
      c->position.z = 0.0f;
      c->color.r = color->r;
      c->color.g = color->g;
      c->color.b = color->b;
      c->color.a = color->a;

      d->position.x = position->x;
      d->position.y = position->y;
      d->position.z = 0.0f;
      d->color.r = color->r;
      d->color.g = color->g;
      d->color.b = color->b;
      d->color.a = color->a;

      EIR_KER_LOG_MESSAGE("Add quad: (%f; %f), (%f; %f), (%f; %f), (%f; %f)",
			  a->position.x, a->position.y,
			  b->position.x, b->position.y,
			  c->position.x, c->position.y,
			  d->position.x, d->position.y
	 );
   }
   return a_handle;
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
	    eir_gfx_api_set_buffer_data(batch);
	    glBindVertexArray(0);
	 }
	 glBindTexture(GL_TEXTURE_2D, batch->texture[0]); // TODO: put in api func file !
	 glUseProgram(gfx_env->sprite_program); // TODO: put in the api func file !
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
	    eir_gfx_api_set_buffer_data(batch);
	    glBindVertexArray(0);
	 }
	 glBindTexture(GL_TEXTURE_2D, batch->texture[0]); // TODO: put in api func file !
	 glUseProgram(gfx_env->text_program); // TODO: put in the api func file !
	 eir_gfx_api_draw_sprite_batch(batch);
	 glUseProgram(0); // TODO: put in api func file !
      }
   }
   if (0 < gfx_env->line_batch.vertices.used)
   {
      if (!gfx_env->line_batch.built)
      {
	 eir_gfx_api_build_vertex_batch(gfx_env, &gfx_env->line_batch);
      }
      glUseProgram(gfx_env->default_program); // TODO: put in the api func file !
      eir_gfx_api_draw_vertex_batch(&gfx_env->line_batch);
      glUseProgram(0); // TODO: put in api func file !
   }
   if (0 < gfx_env->quad_batch.vertices.used)
   {
      if (!gfx_env->quad_batch.built)
      {
	 eir_gfx_api_build_vertex_batch(gfx_env, &gfx_env->quad_batch);
      }
      glUseProgram(gfx_env->default_program); // TODO: put in the api func file !
      eir_gfx_api_draw_vertex_batch(&gfx_env->quad_batch);
      glUseProgram(0); // TODO: put in api func file !
   }
}

void eir_gfx_update_text(eir_gfx_env_t * gfx_env, eir_handle_t text_handle, const char * text)
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

   EIR_KER_RELEASE_ARRAY(batch->sprites);

   int text_len = strlen(text);

   EIR_KER_INIT_ARRAY(eir_gfx_sprite_t, batch->sprites, text_len);
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
      y_offset = MAX_TEXTURE_ROW - ((int)c / (int)MAX_TEXTURE_ROW) - 1;  
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

void eir_gfx_release_all_batches(eir_gfx_env_t * gfx_env)
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
	 eir_gfx_api_release_sprite_batch(batch);
      }
   }
   for (int index = 0; index < gfx_env->text_batches.used; ++index)
   {
      EIR_KER_GET_ARRAY_ITEM(gfx_env->text_batches, index, batch);
      if (batch)
      {
	 eir_gfx_api_release_sprite_batch(batch);
      }
   }
   eir_gfx_api_release_vertex_batch(&gfx_env->line_batch);
   eir_gfx_api_release_vertex_batch(&gfx_env->quad_batch);
}
