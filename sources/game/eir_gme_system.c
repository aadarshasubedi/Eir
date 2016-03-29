#include "eir_gme_system.h"
#include "../fsm/eir_fsm_func.h"
#include "../physics/eir_phy_motion_func.h"
#include "../graphics/eir_gfx_func.h"

static eir_gme_map_layer_t * eir_gme_get_map_layer(
   const eir_gme_world_t * world,
   eir_gme_entity_t map_entity,
   int layer_index
   )
{
   eir_gme_map_layer_t * map_layer = 0;
   eir_gme_map_component_t * map_component = 0;

   if (
      world
      && world->maps.used > map_entity && map_entity >= 0
      )
   {
      eir_gme_entity_flags_t entity_flags = world->entities_flags.data[map_entity];

      if (entity_flags & eir_gme_component_type_map)
      {
         map_component = &world->maps.data[map_entity];
      }
   }
   if (
      map_component
      && map_component->layers.used > layer_index && layer_index >= 0
      )
   {
      map_layer = &map_component->layers.data[layer_index];
   }
   return map_layer;
}

static eir_gme_map_tile_t * eir_gme_get_non_navigable_nearest_map_tile(
   const eir_gme_map_layer_t * map_layer,
   eir_gme_direction_t direction,
   const eir_phy_aabb_t * aabb
   )
{
   eir_gme_map_tile_t * map_tile = 0;

   if (map_layer && aabb && map_layer->tiles.used > 0)
   {
      float left = aabb->position.x;
      float right = left + aabb->size.x;
      float top = aabb->position.y;
      float bottom = top + aabb->size.y;

      for (int i = 0; i < map_layer->tiles.used; ++i)
      {
         eir_gme_map_tile_t * map_tile_iter = &map_layer->tiles.data[i];

         if (
            map_tile_iter
            && map_tile_iter->col_index != -1
            && map_tile_iter->row_index != -1
            && !map_tile_iter->navigable
            )
         {
            float tile_iter_left = map_layer->position.x + map_tile_iter->col_index * map_layer->tile_width;
            float tile_iter_right = tile_iter_left + map_layer->tile_width;
            float tile_iter_top = map_layer->position.y + map_tile_iter->row_index * map_layer->tile_height;
            float tile_iter_bottom = tile_iter_top + map_layer->tile_height;
            float tile_left = 0.0f;
            float tile_right = 0.0f;
            float tile_top = 0.0f;
            float tile_bottom = 0.0f;

            if (map_tile)
            {
               tile_left = map_layer->position.x + map_tile->col_index * map_layer->tile_width;
               tile_right = tile_left + map_layer->tile_width;
               tile_top = map_layer->position.y + map_tile->row_index * map_layer->tile_height;
               tile_bottom = tile_top + map_layer->tile_height;
            }

            if (
               direction == EIR_GME_DIRECTION_RIGHT
               || direction == EIR_GME_DIRECTION_LEFT
               )
            {
               if (top >= tile_iter_bottom || bottom <= tile_iter_top)
               {
                  continue;
               }
               if (
                  direction == EIR_GME_DIRECTION_LEFT
                  && right > tile_iter_right
                  && (map_tile == 0 || (map_tile && tile_right < tile_iter_right))
                  )
               {
                  map_tile = map_tile_iter;
               }
               else if (
                  direction == EIR_GME_DIRECTION_RIGHT
                  && left < tile_iter_left
                  && (map_tile == 0 || (map_tile && tile_left > tile_iter_left))
                  )
               {
                  map_tile = map_tile_iter;
               }
            }
            if (
               direction == EIR_GME_DIRECTION_UP
               || direction == EIR_GME_DIRECTION_BOTTOM
               )
            {
               if (left >= tile_iter_right || right <= tile_iter_left)
               {
                  continue;
               }
               if (
                  direction == EIR_GME_DIRECTION_BOTTOM
                  && bottom < tile_iter_bottom
                  && (map_tile == 0 || (map_tile && tile_top > tile_iter_top))
                  )
               {
                  map_tile = map_tile_iter;
               }
               else if (
                  direction == EIR_GME_DIRECTION_UP
                  && top > tile_iter_top
                  && (map_tile == 0 || (map_tile && tile_bottom < tile_iter_bottom))
                  )
               {
                  map_tile = map_tile_iter;
               }
            }
         }
      }
   }
   return map_tile;
}

static void eir_gme_update_direction_component(eir_gme_direction_component_t * direction_component, float x_velocity, float y_velocity)
{
	if (direction_component)
	{
		eir_gme_direction_t direction = EIR_GME_DIRECTION_UNKNOWN;
		eir_gme_direction_t x_direction = EIR_GME_DIRECTION_UNKNOWN;
		eir_gme_direction_t y_direction = EIR_GME_DIRECTION_UNKNOWN;

		if (eir_mth_abs(x_velocity) > EIR_GME_DIRECTION_EPSILON_VALUE)
		{
			x_direction = x_velocity > 0.0f ? EIR_GME_DIRECTION_RIGHT : EIR_GME_DIRECTION_LEFT;
		}
		if (eir_mth_abs(y_velocity) > EIR_GME_DIRECTION_EPSILON_VALUE)
		{
			y_direction = y_velocity > 0.0f ? EIR_GME_DIRECTION_BOTTOM : EIR_GME_DIRECTION_UP;
		}
		if (x_direction != EIR_GME_DIRECTION_UNKNOWN && y_direction != EIR_GME_DIRECTION_UNKNOWN)
		{
			if (x_direction == EIR_GME_DIRECTION_RIGHT && y_direction == EIR_GME_DIRECTION_UP)
			{
				direction = EIR_GME_DIRECTION_UP_RIGHT;
			}
			if (x_direction == EIR_GME_DIRECTION_RIGHT && y_direction == EIR_GME_DIRECTION_BOTTOM)
			{
				direction = EIR_GME_DIRECTION_BOTTOM_RIGHT;
			}
			if (x_direction == EIR_GME_DIRECTION_LEFT && y_direction == EIR_GME_DIRECTION_UP)
			{
				direction = EIR_GME_DIRECTION_UP_LEFT;
			}
			if (x_direction == EIR_GME_DIRECTION_LEFT && y_direction == EIR_GME_DIRECTION_BOTTOM)
			{
				direction = EIR_GME_DIRECTION_BOTTOM_LEFT;
			}
		}
		else if (x_direction != EIR_GME_DIRECTION_UNKNOWN)
		{
			direction = x_direction;
		}
		else if (y_direction != EIR_GME_DIRECTION_UNKNOWN)
		{
			direction = y_direction;
		}
		direction_component->direction = direction != EIR_GME_DIRECTION_UNKNOWN ? direction : direction_component->direction;
	}
}

static void eir_gme_update_camera(eir_gme_camera_t * cam)
{
   if (cam && cam->target)
   {
      // UPDATE CAM AABB

      eir_mth_vec2_t * cam_pos = &cam->win_aabb.position;
      eir_mth_vec2_t * cam_size = &cam->win_aabb.size;
      eir_mth_vec2_t * target_pos = &cam->target->aabb.position;
      eir_mth_vec2_t * target_size = &cam->target->aabb.size;
      float x = 0.0f;
      float y = 0.0f;

      if (target_pos->x < cam_pos->x)
      {
         x = -(eir_mth_max(eir_mth_abs(cam_pos->x), eir_mth_abs(target_pos->x)) - eir_mth_min(eir_mth_abs(cam_pos->x), eir_mth_abs(target_pos->x)));
      }
      if ((target_pos->x + target_size->x) > (cam_pos->x + cam_size->x))
      {
         x = eir_mth_max(eir_mth_abs(cam_pos->x + cam_size->x), eir_mth_abs(target_pos->x + target_size->x)) - eir_mth_min(eir_mth_abs(cam_pos->x + cam_size->x), eir_mth_abs(target_pos->x + target_size->x));
      }
      if (target_pos->y < cam_pos->y)
      {
         y = -(eir_mth_max(eir_mth_abs(cam_pos->y), eir_mth_abs(target_pos->y)) - eir_mth_min(eir_mth_abs(cam_pos->y), eir_mth_abs(target_pos->y)));
      }
      if ((target_pos->y + target_size->y) > (cam_pos->y + cam_size->y))
      {
         y = eir_mth_max(eir_mth_abs(cam_pos->y + cam_size->y), eir_mth_abs(target_pos->y + target_size->y)) - eir_mth_min(eir_mth_abs(cam_pos->y + cam_size->y), eir_mth_abs(target_pos->y + target_size->y));
      }
      cam_pos->x += x;
      cam_pos->y += y;

      // UPDATE CAM POSITION

      cam->position.x -= cam_pos->x - cam->prev_position.x;
      cam->position.y -= cam_pos->y - cam->prev_position.y;
      cam->prev_position.x = cam_pos->x;
      cam->prev_position.y = cam_pos->y;
   }
}

void eir_gme_start_all_components_systems(eir_gme_world_t * world)
{
	if (world)
	{
		for (int index = 0; index < world->entities_flags.used; ++index)
		{
			eir_gme_entity_flags_t entity_flags = world->entities_flags.data[index];

			if (entity_flags & eir_gme_component_type_fsm)
			{
				eir_gme_fsm_component_t * fsm_component = &world->fsms.data[index];
				eir_fsm_run_state_machine(fsm_component->fsm);
			}
		}
	}
}

void eir_gme_update_all_components_systems(eir_gme_world_t * world, double dtime)
{
	if (world)
	{
      // FIRST WE UPDATE ALL AABB IF POSITION HAS BEEN CHANGED
      // TODO: FIND A MORE ELEGANT TO UPDATE AABB !
		for (int index = 0; index < world->entities_flags.used; ++index)
		{
			eir_gme_entity_flags_t entity_flags = world->entities_flags.data[index];

			if (entity_flags & eir_gme_component_type_aabb)
			{
				eir_gme_aabb_component_t * aabb_component = &world->aabbs.data[index];
				eir_gme_position_component_t * position_component = 0;

				if (entity_flags & eir_gme_component_type_position)
				{
					position_component = &world->positions.data[index];
					if (position_component->modified)
					{
						aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
						aabb_component->modified = true;
					}
            }
         }
      }
		for (int index = 0; index < world->entities_flags.used; ++index)
		{
			eir_gme_entity_flags_t entity_flags = world->entities_flags.data[index];

			if (entity_flags & eir_gme_component_type_fsm)
			{
				eir_gme_fsm_component_t * fsm_component = &world->fsms.data[index];
				eir_fsm_update_state_machine(fsm_component->fsm);
			}
			if (
				(entity_flags & eir_gme_component_type_motion_param)
				&& (entity_flags & eir_gme_component_type_position)
				)
			{
				eir_gme_motion_param_component_t * motion_param_component = &world->motion_params.data[index];
				eir_gme_position_component_t * position_component = &world->positions.data[index];
				eir_mth_vec2_t new_position = { .x = 0.0f, .y = 0.0f };

				eir_phy_proceed_euler_integration(
					&position_component->position,
					&new_position,
					&motion_param_component->motion_param,
					dtime
					);
				eir_gme_set_entity_position(
					world,
					index,
					new_position.x,
					new_position.y
					);
			}
			if (entity_flags & eir_gme_component_type_aabb)
			{
				eir_gme_aabb_component_t * aabb_component = &world->aabbs.data[index];
				eir_gme_position_component_t * position_component = 0;

				if (entity_flags & eir_gme_component_type_position)
				{
					position_component = &world->positions.data[index];
					if (position_component->modified)
					{
						aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
						aabb_component->modified = true;
					}
               if (
                  entity_flags & eir_gme_component_type_map_layer_link
			         && entity_flags & eir_gme_component_type_motion_param
                  )
               {
                  eir_gme_map_layer_link_component_t * map_layer_link = &world->map_layer_links.data[index];
				      eir_gme_motion_param_component_t * motion_param_component = &world->motion_params.data[index];
                  eir_gme_map_layer_t * map_layer = eir_gme_get_map_layer(
                     world,
                     map_layer_link->map_entity,
                     map_layer_link->map_layer_index
                     );

                  if (map_layer)
                  {
                     float layer_left_bound = map_layer->position.x;
                     float layer_right_bound = map_layer->position.x + map_layer->col_count * map_layer->tile_width;
                     float layer_top_bound = map_layer->position.y - aabb_component->aabb.size.y;
                     float layer_bottom_bound = map_layer->position.y + map_layer->col_count * map_layer->tile_height;
                     float entity_left_bound = aabb_component->aabb.position.x;
                     float entity_right_bound = aabb_component->aabb.position.x + aabb_component->aabb.size.x;
                     float entity_top_bound = aabb_component->aabb.position.y;
                     float entity_bottom_bound = aabb_component->aabb.position.y + aabb_component->aabb.size.y;

                     if (entity_top_bound < layer_top_bound)
                     {
                        position_component->position.y += (layer_top_bound - entity_top_bound);
                        aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
						      aabb_component->modified = true;
                     }
                     if (entity_bottom_bound > layer_bottom_bound)
                     {
                        position_component->position.y -= (entity_bottom_bound - layer_bottom_bound);
                        aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
						      aabb_component->modified = true;
                     }
                     if (entity_left_bound < layer_left_bound)
                     {
                        position_component->position.x += (layer_left_bound - entity_left_bound);
                        aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						      aabb_component->modified = true;
                     }
                     if (entity_right_bound > layer_right_bound)
                     {
                        position_component->position.x -= (entity_right_bound - layer_right_bound);
                        aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						      aabb_component->modified = true;
                     }
                     
                     eir_gme_direction_t x_direction = EIR_GME_DIRECTION_UNKNOWN;
                     eir_gme_direction_t y_direction = EIR_GME_DIRECTION_UNKNOWN;
                     float x_velocity = motion_param_component->motion_param.velocity.x;
					      float y_velocity = motion_param_component->motion_param.velocity.y;

		               if (x_velocity != 0.0f)
                     {
                        x_direction = (x_velocity > 0.0f) ? EIR_GME_DIRECTION_RIGHT : EIR_GME_DIRECTION_LEFT;
                     }
		               if (y_velocity != 0.0f)
                     {
                        y_direction = (y_velocity > 0.0f) ? EIR_GME_DIRECTION_BOTTOM : EIR_GME_DIRECTION_UP;
                     }

                     eir_gme_map_tile_t * x_nearest_tile = eir_gme_get_non_navigable_nearest_map_tile(
                        map_layer,
                        x_direction,
                        &aabb_component->aabb
                        );
                     
                     if (x_nearest_tile)
                     {
                        eir_gfx_sprite_proxy_t * sprite_proxy = x_nearest_tile->sprite_proxy;
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
					.g = 0.0f,
					.b = 0.0f,
					.a = 1.0f
				};
                        float nearest_tile_left = map_layer->position.x + x_nearest_tile->col_index * map_layer->tile_width;
                        float nearest_tile_right = nearest_tile_left + map_layer->tile_width;

                        if (
                           x_direction == EIR_GME_DIRECTION_RIGHT
                           && entity_right_bound > nearest_tile_left
                           )
                        {
                           position_component->position.x -= (entity_right_bound - nearest_tile_left);
                           aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						         aabb_component->modified = true;
                        }
                        if (
                           x_direction == EIR_GME_DIRECTION_LEFT
                           && entity_left_bound < nearest_tile_right
                           )
                        {
                           position_component->position.x += (nearest_tile_right - entity_left_bound);
                           aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						         aabb_component->modified = true;
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

                     eir_gme_map_tile_t * y_nearest_tile = eir_gme_get_non_navigable_nearest_map_tile(
                        map_layer,
                        y_direction,
                        &aabb_component->aabb
                        );
                     
                     if (y_nearest_tile)
                     {
                        eir_gfx_sprite_proxy_t * sprite_proxy = y_nearest_tile->sprite_proxy;
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
                        float nearest_tile_top = map_layer->position.y + y_nearest_tile->row_index * map_layer->tile_height;
                        float nearest_tile_bottom = nearest_tile_top + map_layer->tile_height;

                        if (
                           y_direction == EIR_GME_DIRECTION_BOTTOM
                           && entity_bottom_bound > nearest_tile_top
                           )
                        {
                           position_component->position.y -= (entity_bottom_bound - nearest_tile_top);
                           aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
						         aabb_component->modified = true;
                        }
                        if (
                           y_direction == EIR_GME_DIRECTION_UP
                           && entity_top_bound < nearest_tile_bottom
                           )
                        {
                           position_component->position.y += (nearest_tile_bottom - entity_top_bound);
                           aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
						         aabb_component->modified = true;
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
                  }
               }
					if (entity_flags & eir_gme_component_type_physic)
					{
						for (int index2 = 0; index2 < world->entities_flags.used; ++index2)
						{
							if (
								(index != index2)
								&& (world->entities_flags.data[index2] & eir_gme_component_type_aabb)
								&& (world->entities_flags.data[index2] & eir_gme_component_type_physic)
								)
							{
								bool collision = eir_phy_check_aabb_intersection(
									&world->aabbs.data[index].aabb,
									&world->aabbs.data[index2].aabb
									);

								if (collision)
								{
									if (
										world->physics.data[index].weight > 0.0f
										&& world->physics.data[index2].weight > 0.0f
										&& position_component
										)
									{
										float x_depth = eir_phy_get_x_aabb_intersection_depth(
											&world->aabbs.data[index].aabb,
											&world->aabbs.data[index2].aabb
											);
										float y_depth = eir_phy_get_y_aabb_intersection_depth(
											&world->aabbs.data[index].aabb,
											&world->aabbs.data[index2].aabb
											);

										float x = position_component->position.x;
										float y = position_component->position.y;

										if (eir_mth_abs(x_depth) < eir_mth_abs(y_depth))
										{
											x += x_depth;
										}
										else
										{
											y += y_depth;
										}
										eir_gme_set_entity_position(
											world,
											index,
											x,
											y
											);
									}
								}
							}
						}
					}
					// IF COLLISION RESPONSE
					if (position_component->modified)
					{
						aabb_component->aabb.position.x = position_component->position.x + aabb_component->x_offset;
						aabb_component->aabb.position.y = position_component->position.y + aabb_component->y_offset;
						aabb_component->modified = true;
					}
				}
			}
			if (
				(entity_flags & eir_gme_component_type_direction)
				&& (entity_flags & eir_gme_component_type_motion_param)
				)
			{
				eir_gme_direction_component_t * direction_component = &world->directions.data[index];
				eir_gme_motion_param_component_t * motion_param_component = &world->motion_params.data[index];
				eir_gme_update_direction_component(
					direction_component,
					motion_param_component->motion_param.velocity.x,
					motion_param_component->motion_param.velocity.y
					);
			}
			if (
				(entity_flags & eir_gme_component_type_aabb_primitive)
				&& (entity_flags & eir_gme_component_type_aabb)
				)
			{
				eir_gme_aabb_primitive_component_t * aabb_primitive_component = 0;
				eir_gme_aabb_component_t * aabb_component = 0;

				aabb_primitive_component = &world->aabb_primitives.data[index];
				aabb_component = &world->aabbs.data[index];

				eir_gfx_rect_proxy_t * rect_proxy = 0;

				if (aabb_component->modified)
				{
					rect_proxy = aabb_primitive_component->rect_proxy;
				}
				if (rect_proxy)
				{
					eir_mth_vec2_t position =
					{
						.x = rect_proxy->position.x,
						.y = rect_proxy->position.y
					};
					eir_mth_vec2_t size =
					{
						.x = rect_proxy->size.x,
						.y = rect_proxy->size.y
					};
					eir_gfx_color_t color =
					{
						.r = rect_proxy->color.r,
						.g = rect_proxy->color.g,
						.b = rect_proxy->color.b,
						.a = rect_proxy->color.a
					};

					position.x = aabb_component->aabb.position.x;
					position.y = aabb_component->aabb.position.y;
					size.x = aabb_component->aabb.size.x;
					size.y = aabb_component->aabb.size.y;

					eir_gfx_modify_rect(
					   rect_proxy,
					   &position,
					   &size,
					   &color
					   );
				}
			}
			if (entity_flags & eir_gme_component_type_sprite)
			{
				eir_gme_sprite_component_t * sprite_component = &world->sprites.data[index];
				eir_gfx_sprite_proxy_t * sprite_proxy = sprite_component->sprite_proxy;


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
					.r = sprite_proxy->color.r,
					.g = sprite_proxy->color.g,
					.b = sprite_proxy->color.b,
					.a = sprite_proxy->color.a
				};

				bool must_modify_sprite = false;

				if (entity_flags & eir_gme_component_type_position)
				{
					eir_gme_position_component_t * position_component = &world->positions.data[index];

					if (position_component->modified)
					{
						position.x = position_component->position.x;
						position.y = position_component->position.y;
						position_component->modified = false;
						must_modify_sprite = true;
					}
				}
				if (entity_flags & eir_gme_component_type_size)
				{
					eir_gme_size_component_t * size_component = &world->sizes.data[index];

					if (size_component->modified)
					{
						size.x = size_component->size.x;
						size.y = size_component->size.y;
						must_modify_sprite = true;
					}
				}
				if (entity_flags & eir_gme_component_type_color)
				{
					eir_gme_color_component_t * color_component = &world->colors.data[index];

					if (color_component->modified)
					{
						color.r = color_component->color.r;
						color.g = color_component->color.g;
						color.b = color_component->color.b;
						color.a = color_component->color.a;
						must_modify_sprite = true;
					}
				}
				if (must_modify_sprite)
				{
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

			// CLEAM MODIFIED FLAGS
			
			if (entity_flags & eir_gme_component_type_position)
			{
				world->positions.data[index].modified = false;
			}
			if (entity_flags & eir_gme_component_type_size)
			{
				world->sizes.data[index].modified = false;
			}
			if (entity_flags & eir_gme_component_type_color)
			{
				world->colors.data[index].modified = false;
			}
			if (entity_flags & eir_gme_component_type_aabb)
			{
				world->aabbs.data[index].modified = false;
			}
		}

		// ALWAYS FINISH WITH CAMERA UPDATE !

		eir_gme_update_camera(&world->camera);
	}
}
