#include "eir_gfx_func.h"
#include "eir_gfx_api_func.h"

void eir_gfx_set_sprite_batch_capacity(eir_gfx_env_t * gfx_env, int max_capacity)
{
   if (gfx_env)
   {
      EIR_KER_INIT_ARRAY(eir_gfx_sprite_batch_t, gfx_env->sprite_batches, max_capacity);
   }
}

eir_gfx_sprite_batch_t * eir_gfx_create_empty_sprite_batch(eir_gfx_env_t * gfx_env, int max_capacity)
{
   eir_gfx_sprite_batch_t * sprite_batch = 0;

   if (gfx_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(gfx_env->sprite_batches, sprite_batch);
   }
   if (sprite_batch)
   {
      EIR_KER_INIT_ARRAY(eir_gfx_sprite_t, sprite_batch->sprites, max_capacity);
      sprite_batch->built = false;
      sprite_batch->modified = true;
   }
   return sprite_batch;
}

void eir_gfx_add_sprite_to_batch(
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   eir_gfx_color_t * color,
   eir_gfx_sprite_batch_t * batch
   )
{
   // TODO: check all pointers
   if (!batch)
   {
      return;
   }
   
   eir_gfx_sprite_t * sprite = 0;

   EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(batch->sprites, sprite);
   if (sprite)
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
      
   }
   batch->modified = true;
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
	    eir_gfx_api_build_sprite_batch(batch);
	 }
	 else if (batch->modified)
	 {
	    eir_gfx_api_set_buffer_data(batch);
	 }
	 eir_gfx_api_draw_sprite_batch(batch);
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
	 eir_gfx_api_release_batch(batch);
      }
   }
}

