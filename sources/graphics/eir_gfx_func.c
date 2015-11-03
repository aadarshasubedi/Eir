#include "eir_gfx_func.h"

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
   }
   return sprite_batch;
}

void eir_gfx_add_sprite_to_batch(
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * size,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   eir_gfx_sprite_batch_t * batch
   )
{
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
   }
}
