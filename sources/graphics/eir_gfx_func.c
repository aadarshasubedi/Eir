#include "eir_gfx_func.h"

void eir_gfx_add_sprite_to_batch(
   eir_mth_vec2_t * position,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size,
   eir_gfx_sprite_batch_t * batch
   )
{
   if (0 == batch)
   {
      return;
   }
   if (batch->curr_sprites_count >= MAX_SPRITES_PER_BATCH)
   {
      return;
   }

   int index = batch->curr_sprites_count;

   batch->sprites[index].position.x = position->x;
   batch->sprites[index].position.y = position->y;
   batch->sprites[index].uv_offset.x = uv_offset->x;
   batch->sprites[index].uv_offset.y = uv_offset->y;
   batch->sprites[index].uv_size.x = uv_size->x;
   batch->sprites[index].uv_size.y = uv_size->y;
   
   ++batch->curr_sprites_count;
}
