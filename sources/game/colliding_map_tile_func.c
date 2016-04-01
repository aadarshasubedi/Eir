#include "colliding_map_tile_func.h"
#include "../kernel/eir_ker_log.h"
#include "../system/eir_sys_memory.h"
#include "../graphics/eir_gfx_func.h"

// ----------------------------------------------------------------------------
void init_colliding_map_tile(colliding_map_tile_t * colliding_map_tile)
{
   if (colliding_map_tile)
   {
      colliding_map_tile->map_tile_ptr = 0;
      colliding_map_tile->intersection_area = 0.0f;
   }
}

// ----------------------------------------------------------------------------
void init_colliding_map_tile_array(colliding_map_tile_array_t * array)
{
   if (array)
   {
      array->data = 0;
      array->max_capacity = 0;
      array->used = 0;
   }
}

// ----------------------------------------------------------------------------
void alloc_colliding_map_tile_array(colliding_map_tile_array_t * array, size_t max_capacity)
{
   if (
      array
      && max_capacity > 0
      && array->data == 0
      )
   {
      EIR_SYS_ALLOC(array->data, sizeof(colliding_map_tile_t), max_capacity);
      array->max_capacity = max_capacity;
      array->used = 0;
      for (int i = 0; i < max_capacity; ++i)
      {
         init_colliding_map_tile(&array->data[i]);
      }
   }
}

// ----------------------------------------------------------------------------
void swap_colliding_map_tiles(colliding_map_tile_array_t * array, size_t tile_id_1, size_t tile_id_2)
{
   if (
      array
      && array->data
      && tile_id_1 < array->max_capacity
      && tile_id_2 < array->max_capacity
      )
   {
      eir_gme_map_tile_t * map_tile_ptr_tmp = array->data[tile_id_1].map_tile_ptr;
      float intersection_area_tmp = array->data[tile_id_1].intersection_area;

      array->data[tile_id_1].map_tile_ptr = array->data[tile_id_2].map_tile_ptr;
      array->data[tile_id_1].intersection_area = array->data[tile_id_2].intersection_area;
      array->data[tile_id_2].map_tile_ptr = map_tile_ptr_tmp;
      array->data[tile_id_2].intersection_area = intersection_area_tmp;
   }
}

// ----------------------------------------------------------------------------
void use_colliding_map_tile(colliding_map_tile_array_t * array, size_t tile_id)
{
   if (
      array
      && array->data
      && tile_id >= array->used && tile_id < array->max_capacity
      )
   {
      swap_colliding_map_tiles(array, tile_id, array->used);
      ++array->used; 
   }
}

// ----------------------------------------------------------------------------
void unuse_colliding_map_tile(colliding_map_tile_array_t * array, size_t tile_id)
{
   if (
      array
      && array->data
      && tile_id < array->used
      )
   {
      swap_colliding_map_tiles(array, tile_id, array->used - 1);
      --array->used;
   }
}

// ----------------------------------------------------------------------------
int push_colliding_map_tile(colliding_map_tile_array_t * array, eir_gme_map_tile_t * map_tile, float intersection_area)
{
   int index = -1;

   if (
      array
      && map_tile
      && array->data
      && array->used < array->max_capacity
      )
   {
      int index = array->used;

      array->data[index].map_tile_ptr = map_tile;
      array->data[index].intersection_area = intersection_area;
      index = array->used;
      ++array->used;

#ifdef EIR_DEBUG
      eir_gfx_sprite_proxy_t * sprite_proxy = map_tile->sprite_proxy;

      if (sprite_proxy)
      {
			eir_mth_vec2_t position =
			{
				.x = sprite_proxy->position.x,
				.y = sprite_proxy->position.y
			};
			eir_mth_vec2_t size =
			{
				.x = sprite_proxy->size.x,
				.y = sprite_proxy->size.y
			};
			eir_mth_vec2_t uv_offset =
		   {
				.x = sprite_proxy->uv_offset.x,
				.y = sprite_proxy->uv_offset.y
			};
			eir_mth_vec2_t uv_size =
			{
				.x = sprite_proxy->uv_size.x,
				.y = sprite_proxy->uv_size.y
			};

			eir_gfx_color_t color =
			{
				.r = 0.0f,
				.g = 0.0f,
				.b = 1.0f,
				.a = 1.0f
			};
         if (!map_tile->navigable)
         {
				color.r = 1.0f;
				color.g = 0.0f;
				color.b = 0.0f;
				color.a = 1.0f;
         }
			eir_gfx_modify_sprite(
	   		sprite_proxy,
	   		&position,
	   		&size,
	   		&uv_offset,
	   	   &uv_size,
	   		&color
	   		);
      }
#endif

   }
   else if (
      array
      && array->used == array->max_capacity
      )
   {
      EIR_KER_LOG_ERROR("CANNOT PUSH MORE COLLIDING MAP TILE");
   }
   return index;
}

// ----------------------------------------------------------------------------
void sort_colliding_map_tile_array(colliding_map_tile_array_t * array)
{
   if (array)
   {
      // WE SET TO UMUSED MAVIGABLE TILES

      for (int i = 0; i < array->max_capacity; ++i)
      {
         colliding_map_tile_t * colliding_map_tile = &array->data[i];

         if (
            colliding_map_tile
            && colliding_map_tile->map_tile_ptr
            && colliding_map_tile->map_tile_ptr->navigable
            )
         {
            unuse_colliding_map_tile(array, i);
         }
      }

      // WE SORT TILES BY INTERSECTION AREA : BIGGER TO SMALLER

   }
}

// ----------------------------------------------------------------------------
void empty_colliding_map_tile_array(colliding_map_tile_array_t * array)
{
   if (array)
   {
#ifdef EIR_DEBUG
      for (int i = 0; i < array->max_capacity; ++i)
      {
         if (!array->data[i].map_tile_ptr)
         {
            continue;
         }
         eir_gfx_sprite_proxy_t * sprite_proxy = array->data[i].map_tile_ptr->sprite_proxy;

         if (sprite_proxy)
         {
			   eir_mth_vec2_t position =
			   {
				   .x = sprite_proxy->position.x,
				   .y = sprite_proxy->position.y
			   };
			   eir_mth_vec2_t size =
			   {
				   .x = sprite_proxy->size.x,
				   .y = sprite_proxy->size.y
			   };
			   eir_mth_vec2_t uv_offset =
		      {
				   .x = sprite_proxy->uv_offset.x,
				   .y = sprite_proxy->uv_offset.y
			   };
			   eir_mth_vec2_t uv_size =
			   {
				   .x = sprite_proxy->uv_size.x,
				   .y = sprite_proxy->uv_size.y
			   };
			   eir_gfx_color_t color =
			   {
				   .r = 1.0f,
				   .g = 1.0f,
				   .b = 1.0f,
				   .a = 1.0f
			   };
				eir_gfx_modify_sprite(
	   			sprite_proxy,
	   			&position,
	   			&size,
	   			&uv_offset,
	   			&uv_size,
	   			&color
	   			);
         }
      }
#endif

      array->used = 0;
   }
}

// ----------------------------------------------------------------------------
void free_colliding_map_tile_array(colliding_map_tile_array_t * array)
{
   if (array)
   {
      EIR_SYS_FREE(array->data);
      array->data = 0;
      array->max_capacity = 0;
      array->used = 0;
   }
}

// ----------------------------------------------------------------------------
void init_colliding_map_tile_array_array(colliding_map_tile_array_array_t * array)
{
   if (array)
   {
      array->data = 0;
      array->max_capacity = 0;
      array->used = 0;
   }
}

// ----------------------------------------------------------------------------
void alloc_colliding_map_tile_array_array(colliding_map_tile_array_array_t * array, size_t max_capacity)
{
   if (
      array
      && array->data == 0
      && max_capacity > 0
      )
   {
      EIR_SYS_ALLOC(array->data, sizeof (colliding_map_tile_array_t), max_capacity);
      array->max_capacity = max_capacity;
      array->used = 0;
      for (int i = 0; i < max_capacity; ++i)
      {
         init_colliding_map_tile_array(&array->data[i]);
      }
   }
}

// ----------------------------------------------------------------------------
colliding_map_tile_array_t * get_unused_colliding_map_tile_array(colliding_map_tile_array_array_t * array)
{
   colliding_map_tile_array_t * result = 0;

   if (
      array
      && array->data
      && array->used < array->max_capacity
      )
   {
      result = &array->data[array->used];
      ++array->used;
   }
   else
   {
      if (array->used == array->max_capacity)
      {
         EIR_KER_LOG_ERROR("NO UNUSED COLLIDING MAP TILE AVAILABLE, REACHED %d/%d", array->used, array->max_capacity);
      }
   }
   return result;
}

// ----------------------------------------------------------------------------
void free_colliding_map_tile_array_array(colliding_map_tile_array_array_t * array)
{
   if (array)
   {
      for (int i = 0; i < array->max_capacity; ++i)
      {
         free_colliding_map_tile_array(&array->data[i]);
      }
      EIR_SYS_FREE(array->data);
      array->data = 0;
      array->max_capacity = 0;
      array->used = 0;
   }
}
