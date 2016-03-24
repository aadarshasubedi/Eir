#include "eir_gme_func.h"
#include "../kernel/eir_ker_env.h"
#include "../kernel/eir_ker_log.h"
#include "../system/eir_sys_joystick_func.h"
#include "../maths/eir_mth_func.h"
#include "../graphics/eir_gfx_func.h"

static void eir_gme_init_camera(eir_gme_camera_t * camera)
{
   if (camera)
   {
      camera->position.x = 0.0f;
      camera->position.y = 0.0f;
      camera->prev_position.x = 0.0f;
      camera->prev_position.y = 0.0f;
      camera->target = 0;
      camera->win_aabb.position.x = 0.0f;
      camera->win_aabb.position.y = 0.0f;
      camera->win_aabb.size.x = 0.0f;
      camera->win_aabb.size.y = 0.0f;
      camera->win_scale = 0.0f;
      camera->viewport_w = 0;
      camera->viewport_h = 0;
   }
}

static void eir_gme_init_world(eir_gme_world_t * world)
{
   if (world)
   {
      EIR_KER_INIT_ARRAY(world->entities_flags);
      EIR_KER_INIT_ARRAY(world->positions);
      EIR_KER_INIT_ARRAY(world->sizes);
      EIR_KER_INIT_ARRAY(world->sprites);
      EIR_KER_INIT_ARRAY(world->colors);
      EIR_KER_INIT_ARRAY(world->motion_params);
      EIR_KER_INIT_ARRAY(world->aabbs);
      EIR_KER_INIT_ARRAY(world->physics);
      EIR_KER_INIT_ARRAY(world->states);
      EIR_KER_INIT_ARRAY(world->fsms);
      EIR_KER_INIT_ARRAY(world->aabb_primitives);
      EIR_KER_INIT_ARRAY(world->keyboards);
      EIR_KER_INIT_ARRAY(world->pads);
      EIR_KER_INIT_ARRAY(world->maps);
      eir_gme_init_camera(&world->camera);
   }
}

static void eir_gme_release_world(eir_gme_world_t * world)
{
   if (world)
   {
      EIR_KER_FREE_ARRAY(world->entities_flags);
      EIR_KER_FREE_ARRAY(world->positions);
      EIR_KER_FREE_ARRAY(world->sizes);
      EIR_KER_FREE_ARRAY(world->sprites);
      EIR_KER_FREE_ARRAY(world->colors);
      EIR_KER_FREE_ARRAY(world->motion_params);
      EIR_KER_FREE_ARRAY(world->aabbs);
      EIR_KER_FREE_ARRAY(world->physics);
      EIR_KER_FREE_ARRAY(world->directions);
      EIR_KER_FREE_ARRAY(world->states);
      EIR_KER_FREE_ARRAY(world->fsms);
      EIR_KER_FREE_ARRAY(world->aabb_primitives);
      EIR_KER_FREE_ARRAY(world->keyboards);
      EIR_KER_FREE_ARRAY(world->pads);
      EIR_KER_FREE_ARRAY(world->maps);
      eir_gme_init_camera(&world->camera);
   }
}

static void eir_gme_init_entity_flags(eir_gme_entity_flags_t * entity_flags)
{
   if (entity_flags)
   {
      *entity_flags = eir_gme_component_type_none;
   }
}

static void eir_gme_release_entity_flags(eir_gme_entity_flags_t * entity_flags)
{
   eir_gme_init_entity_flags(entity_flags);
}

static void eir_gme_init_position(eir_gme_position_component_t * position)
{
   if (position)
   {
      position->position.x = 0.0f;
      position->position.y = 0.0f;
      position->modified = false;
   }
}

static void eir_gme_release_position(eir_gme_position_component_t * position)
{
   eir_gme_init_position(position);
}

static void eir_gme_init_size(eir_gme_size_component_t * size)
{
   if (size)
   {
      size->size.x = 0.0f;
      size->size.y = 0.0f;
      size->modified = false;
   }
}

static void eir_gme_release_size(eir_gme_size_component_t * size)
{
   eir_gme_init_size(size);
}

static void eir_gme_init_sprite(eir_gme_sprite_component_t * sprite)
{
   if (sprite)
   {
      sprite->sprite_proxy = 0;
   }
}

static void eir_gme_release_sprite(eir_gme_sprite_component_t * sprite)
{
   eir_gme_init_sprite(sprite);
}

static void eir_gme_init_color(eir_gme_color_component_t * color)
{
   if (color)
   {
      color->color.r = 1.0f;
      color->color.g = 1.0f;
      color->color.b = 1.0f;
      color->color.a = 1.0f;
      color->modified = false;
   }
}

static void eir_gme_release_color(eir_gme_color_component_t * color)
{
   eir_gme_init_color(color);
}

static void eir_gme_init_motion_param(
   eir_gme_motion_param_component_t * motion_param
   )
{
   if (motion_param)
   {
      motion_param->motion_param.velocity.x = 0.0f;
      motion_param->motion_param.velocity.y = 0.0f;
      motion_param->motion_param.acceleration.x = 0.0f;
      motion_param->motion_param.acceleration.y = 0.0f;
      motion_param->motion_param.speed_factor = 1.0f;
      motion_param->motion_param.friction_factor = 0.0f;
   }
}

static void eir_gme_release_motion_param(
   eir_gme_motion_param_component_t * motion_param
   )
{
   eir_gme_init_motion_param(motion_param);
}

static void eir_gme_init_aabb(eir_gme_aabb_component_t * aabb)
{
   if (aabb)
   {
      aabb->aabb.position.x = 0.0f;
      aabb->aabb.position.x = 0.0f;
      aabb->aabb.size.x = 0.0f;
      aabb->aabb.size.y = 0.0f;
      aabb->x_offset = 0.0f;
      aabb->y_offset = 0.0f;
      aabb->modified = true;
   }
}

static void eir_gme_release_aabb(eir_gme_aabb_component_t * aabb)
{
   eir_gme_init_aabb(aabb);
}

static void eir_gme_init_physic(eir_gme_physic_component_t * physic)
{
   if (physic)
   {
      physic->weight = 0.0f;
   }
}

static void eir_gme_release_physic(eir_gme_physic_component_t * physic)
{
   eir_gme_init_physic(physic);
}

static void eir_gme_init_direction(eir_gme_direction_component_t * direction)
{
	if (direction)
	{
		direction->direction = EIR_GME_DIRECTION_UNKNOWN;
	}
}

static void eir_gme_release_direction(eir_gme_direction_component_t * direction)
{
	eir_gme_init_direction(direction);
}

static void eir_gme_init_state(eir_gme_state_component_t * state)
{
	if (state)
	{
		state->visible = true;
		state->alive = true;
	}
}

static void eir_gme_release_state(eir_gme_state_component_t * state)
{
	eir_gme_init_state(state);
}

static void eir_gme_init_fsm(eir_gme_fsm_component_t * fsm)
{
   if (fsm)
   {
      fsm->fsm = 0;
   }
}

static void eir_gme_release_fsm(eir_gme_fsm_component_t * fsm)
{
   eir_gme_init_fsm(fsm);
}

static void eir_gme_init_aabb_primitive(eir_gme_aabb_primitive_component_t * aabb_primitive)
{
   if (aabb_primitive)
   {
      aabb_primitive->rect_proxy = 0;
   }
}

static void eir_gme_release_aabb_primitive(eir_gme_aabb_primitive_component_t * aabb_primitive)
{
   eir_gme_init_aabb_primitive(aabb_primitive);
}

static void eir_gme_init_keyboard(eir_gme_keyboard_component_t * keyboard)
{
   if (keyboard)
   {
      keyboard->input_buffer = 0;
   }
}

static void eir_gme_release_keyboard(eir_gme_keyboard_component_t * keyboard)
{
   eir_gme_init_keyboard(keyboard);
}

static void eir_gme_init_pad(eir_gme_pad_component_t * pad)
{
   if (pad)
   {
      pad->input_buffer = 0;
   }
}

static void eir_gme_release_pad(eir_gme_pad_component_t * pad)
{
   eir_gme_init_pad(pad);
}

static void eir_gme_init_map(eir_gme_map_component_t * map)
{
   if (map)
   {
      map->col_count = 0;
      map->row_count = 0;
      map->tile_width = 0;
      map->tile_height = 0;
      map->batch = 0;
      EIR_KER_INIT_ARRAY(map->tiles);
   }
}
static void eir_gme_release_map(eir_gme_map_component_t * map)
{
   if (map)
   {
      map->col_count = 0;
      map->row_count = 0;
      map->tile_width = 0;
      map->tile_height = 0;
      map->batch = 0;
      EIR_KER_FREE_ARRAY(map->tiles);
   }
}

static void eir_gme_init_map_tile(eir_gme_map_tile_t * map_tile)
{
   if (map_tile)
   {
      map_tile->col_index = -1;
      map_tile->row_index = -1;
   }
}

static void eir_gme_init_button_state(eir_gme_button_state_t * button_state)
{
   if (button_state)
   {
      button_state->pressed = false;
   }
}

static void eir_gme_init_input_controller(
   eir_gme_input_controller_t * input_controller,
   bool is_connected
   )
{
   if (input_controller)
   {
      input_controller->is_connected = is_connected;
      input_controller->is_analog = false;
      input_controller->left_stick_value_x = 0.0f;
      input_controller->left_stick_value_y = 0.0f;
      for (int index = 0; index < EIR_GME_TOTAL_INPUT_BUTTON_COUNT; ++index)
      {
         eir_gme_init_button_state(&input_controller->buttons[index]);
      }
   }
}

static void eir_gme_init_input_controller_buffer(
   eir_gme_input_controller_buffer_t * controller_buffer,
   bool is_connected
   )
{
   if (controller_buffer)
   {
      int count = EIR_GME_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT;
      for (int index = 0; index < count; ++index)
      {
         eir_gme_init_input_controller(
            &controller_buffer->controllers[index],
            is_connected
            );
      }
   }
}

static void eir_gme_init_all_input_controller_buffer(eir_gme_env_t * env)
{
   if (env)
   {
      for (int index = 0; index < EIR_GME_TOTAL_INPUT_CONTROLLER; ++index)
      {
         bool is_connected = (index == 0 || index <= eir_sys_get_pad_count());
         eir_gme_init_input_controller_buffer(
            &env->input_controllers[index],
            is_connected
            );
      }
   }
}

void eir_gme_init_env(eir_gme_env_t * env)
{
   EIR_KER_LOG_MESSAGE("init game env");
   if (env)
   {
      EIR_KER_INIT_ARRAY(env->worlds);
      env->curr_world = 0;
      eir_gme_init_all_input_controller_buffer(env);
   }
}

void eir_gme_release_env(eir_gme_env_t * env)
{
   EIR_KER_LOG_MESSAGE("release game env");
   if (env)
   {
      EIR_KER_FREE_ARRAY_BIS(env->worlds, eir_gme_release_world);
      env->curr_world = 0;
   }
}

void eir_gme_set_world_capacity(eir_gme_env_t * env, size_t max_count)
{
   if (env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_world_t,
         env->worlds,
         max_count,
         eir_gme_init_world
         )
   }
}

eir_gme_world_t * eir_gme_create_world(
   eir_gme_env_t * env,
   size_t max_entity_count
   )
{
   eir_gme_world_t * world = 0;

   if (env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->worlds, world);
   }
   if (world)
   {  
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_entity_flags_t,
         world->entities_flags,
         max_entity_count,
         eir_gme_init_entity_flags
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_position_component_t,
         world->positions,
         max_entity_count,
         eir_gme_init_position
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_size_component_t,
         world->sizes,
         max_entity_count,
         eir_gme_init_size
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_sprite_component_t,
         world->sprites,
         max_entity_count,
         eir_gme_init_sprite
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_color_component_t,
         world->colors,
         max_entity_count,
         eir_gme_init_color
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_motion_param_component_t,
         world->motion_params,
         max_entity_count,
         eir_gme_init_motion_param
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_aabb_component_t,
         world->aabbs,
         max_entity_count,
         eir_gme_init_aabb
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_physic_component_t,
         world->physics,
         max_entity_count,
         eir_gme_init_physic
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_direction_component_t,
         world->directions,
         max_entity_count,
         eir_gme_init_direction
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_state_component_t,
         world->states,
         max_entity_count,
         eir_gme_init_state
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_fsm_component_t,
         world->fsms,
         max_entity_count,
         eir_gme_init_fsm
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_aabb_primitive_component_t,
         world->aabb_primitives,
         max_entity_count,
         eir_gme_init_aabb_primitive
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_keyboard_component_t,
         world->keyboards,
         max_entity_count,
         eir_gme_init_keyboard
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_pad_component_t,
         world->pads,
         max_entity_count,
         eir_gme_init_pad
         );
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_map_component_t,
         world->maps,
         max_entity_count,
         eir_gme_init_map
         );
   }
   return world;
}

eir_gme_entity_t eir_gme_create_world_entity(eir_gme_world_t * world)
{
   eir_gme_entity_t entity = EIR_GME_INVALID_ENTITY;

   if (world)
   {
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->entities_flags, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->positions, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->sizes, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->sprites, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->colors, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->motion_params, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->aabbs, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->physics, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->directions, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->states, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->fsms, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->aabb_primitives, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->keyboards, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->pads, entity);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->maps, entity);
   }
   return entity;
}

void eir_gme_set_entity_position(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   float x,
   float y
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_position_component_t * position_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->positions, entity, position_component);
   }
   if (entity_flags && position_component)
   {
      (*entity_flags) |= eir_gme_component_type_position;
      position_component->position.x = x;
      position_component->position.y = y;
      position_component->modified = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_size(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   float width,
   float height
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_size_component_t * size_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->sizes, entity, size_component);
   }
   if (entity_flags && size_component)
   {
      (*entity_flags) |= eir_gme_component_type_size;
      size_component->size.x = width;
      size_component->size.y = height;
      size_component->modified = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_sprite(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gfx_sprite_proxy_t * sprite_proxy
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_sprite_component_t * sprite_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->sprites, entity, sprite_component);
   }
   if (entity_flags && sprite_component)
   {
      (*entity_flags) |= eir_gme_component_type_sprite;
      sprite_component->sprite_proxy = sprite_proxy;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_color(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   float r,
   float g,
   float b,
   float a
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_color_component_t * color_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->colors, entity, color_component);
   }
   if (entity_flags && color_component)
   {
      (*entity_flags) |= eir_gme_component_type_color;
      color_component->color.r = r;
      color_component->color.g = g;
      color_component->color.b = b;
      color_component->color.a = a;
      color_component->modified = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_acceleration(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   float ax,
   float ay,
   float speed,
   float friction
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_motion_param_component_t * motion_param_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(
         world->motion_params,
         entity,
         motion_param_component
         );
   }
   if (entity_flags && motion_param_component)
   {
      (*entity_flags) |= eir_gme_component_type_position;
      (*entity_flags) |= eir_gme_component_type_motion_param;
      motion_param_component->motion_param.acceleration.x = ax;
      motion_param_component->motion_param.acceleration.y = ay;
      motion_param_component->motion_param.speed_factor = speed;
      motion_param_component->motion_param.friction_factor = friction;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_aabb(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   float x_offset,
   float y_offset,
   float width,
   float height
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_aabb_component_t * aabb_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->aabbs, entity, aabb_component);
   }
   if (entity_flags && aabb_component)
   {
      (*entity_flags) |= eir_gme_component_type_aabb;
      aabb_component->aabb.position.x = x_offset;
      aabb_component->aabb.position.y = y_offset;
      aabb_component->aabb.size.x = width;
      aabb_component->aabb.size.y = height;
      aabb_component->x_offset = x_offset;
      aabb_component->y_offset = y_offset;
      aabb_component->modified = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

/*
void eir_gme_set_entity_camera(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   float win_scale,
   int viewport_w,
   int viewport_h
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_camera_component_t * camera_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->cameras, entity, camera_component);
   }
   if (entity_flags && camera_component)
   {
      (*entity_flags) |= eir_gme_component_type_camera;
      camera_component->win_scale = win_scale;
      camera_component->viewport_w = viewport_w;
      camera_component->viewport_h = viewport_h;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity);
   }
}
*/

void eir_gme_set_entity_physic(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   float weight
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_physic_component_t * physic_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->physics, entity, physic_component);
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
   }
   if (entity_flags && physic_component)
   {
      (*entity_flags) |= eir_gme_component_type_position;
      (*entity_flags) |= eir_gme_component_type_physic;
      physic_component->weight = weight;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity);
   }
}

void eir_gme_set_entity_direction(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gme_direction_t direction
   )
{
	eir_gme_entity_flags_t * entity_flags = 0;
	eir_gme_direction_component_t * direction_component = 0;

	if (world)
	{
		EIR_KER_GET_ARRAY_ITEM(world->directions, entity, direction_component);
		EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
	}
	if (entity_flags && direction_component)
	{
		(*entity_flags) |= eir_gme_component_type_direction;
		direction_component->direction = direction;
	}
	else
	{
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity);
   }
}

void eir_gme_set_entity_state(
	eir_gme_world_t * world,
	eir_gme_entity_t entity,
	bool visible,
	bool alive
	)
{
	eir_gme_entity_flags_t * entity_flags = 0;
	eir_gme_state_component_t * state_component = 0;

	if (world)
	{
		EIR_KER_GET_ARRAY_ITEM(world->states, entity, state_component);
		EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
	}
	if (entity_flags && state_component)
	{
		(*entity_flags) |= eir_gme_component_type_state;
		state_component->visible = visible;
		state_component->alive = alive;
	}
	else
	{
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity);
   }
}

void eir_gme_set_entity_fsm(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_fsm_state_machine_t * fsm
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_fsm_component_t * fsm_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->fsms, entity, fsm_component);
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
   }
   if (entity_flags && fsm_component)
   {
      (*entity_flags) |= eir_gme_component_type_fsm;
      fsm_component->fsm = fsm;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity);
   }
}

void eir_gme_set_entity_aabb_primitive(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gfx_rect_proxy_t * rect_proxy
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_aabb_primitive_component_t * aabb_primitive_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->aabb_primitives, entity, aabb_primitive_component);
   }
   if (entity_flags && aabb_primitive_component)
   {
      (*entity_flags) |= eir_gme_component_type_aabb_primitive;
      aabb_primitive_component->rect_proxy = rect_proxy;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_keyboard_controller(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gme_input_controller_buffer_t * input_buffer
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_keyboard_component_t * keyboard_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->keyboards, entity, keyboard_component);
   }
   if (entity_flags && keyboard_component)
   {
      (*entity_flags) |= eir_gme_component_type_keyboard_controller;
      keyboard_component->input_buffer = input_buffer;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_pad_controller(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gme_input_controller_buffer_t * input_buffer
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_pad_component_t * pad_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->pads, entity, pad_component);
   }
   if (entity_flags && pad_component)
   {
      (*entity_flags) |= eir_gme_component_type_pad_controller;
      pad_component->input_buffer = input_buffer;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_map(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   eir_gfx_sprite_batch_t * batch,
   int col_count,
   int row_count,
   int tile_width,
   int tile_height,
   size_t tiles_capacity
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_map_component_t * map_component = 0;

   if (world && batch)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->maps, entity, map_component);
   }
   if (entity_flags && map_component)
   {
      (*entity_flags) |= eir_gme_component_type_map;
      map_component->col_count = col_count;
      map_component->row_count = row_count;
      map_component->tile_width = tile_width;
      map_component->tile_height = tile_height;
      map_component->batch = batch;
      EIR_KER_ALLOCATE_ARRAY_BIS(
         eir_gme_map_tile_t,
         map_component->tiles,
         tiles_capacity,
         eir_gme_init_map_tile
         );
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
}

void eir_gme_set_entity_map_tile(
   eir_gme_world_t * world,
   eir_gme_entity_t entity,
   int col_index,
   int row_index,
   eir_mth_vec2_t * uv_offset,
   eir_mth_vec2_t * uv_size
   )
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_map_component_t * map_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->maps, entity, map_component);
   }

   eir_gme_map_tile_t * map_tile = 0;

   if (
      entity_flags
      && (*entity_flags) & eir_gme_component_type_map
      && map_component
      )
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(map_component->tiles, map_tile);
      if (
         map_tile
         && col_index >= 0 && col_index < map_component->col_count
         && row_index >= 0 && row_index < map_component->row_count
         )
      {
         map_tile->col_index = col_index;
         map_tile->row_index = row_index;
         
         eir_mth_vec2_t position;
         eir_mth_vec2_t size;
         eir_gfx_color_t color;
         
         position.x = col_index * size.x;
         position.y = row_index * size.y;
         size.x = map_component->tile_width;
         size.y = map_component->tile_height;
         color.r = 1.0f;
         color.g = 1.0f;
         color.b = 1.0f;
         color.a = 1.0f;

         eir_gfx_add_sprite_to_batch(
            map_component->batch,
            &position,
            &size,
            uv_offset,
            uv_size,
            &color,
            true
            );
      }
   }
}

void eir_gme_set_active_camera(
   eir_gme_world_t * world,
   eir_gme_entity_t target,
   float win_scale,
   int viewport_w,
   int viewport_h
   )
{
   if (world)
   {
      eir_gme_entity_flags_t * entity_flags = 0;
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, target, entity_flags);

      if (
         ((*entity_flags) & eir_gme_component_type_aabb)
         && ((*entity_flags) & eir_gme_component_type_position)
         )
      {
         eir_gme_aabb_component_t * aabb = &world->aabbs.data[target];
         eir_gme_position_component_t * pos = &world->positions.data[target];
         eir_gme_camera_t * cam = &world->camera;


         cam->win_scale = win_scale;
         cam->viewport_w = viewport_w;
         cam->viewport_h = viewport_h;
         // TODO: FIX THE MAGIC VALUE...
         cam->position.x =
            (float)cam->viewport_w * 0.233f - pos->position.x - aabb->x_offset - aabb->aabb.size.x * 0.5f;
         cam->position.y =
            (float)cam->viewport_h * 0.233f - pos->position.y - aabb->y_offset - aabb->aabb.size.y * 0.5f;
         //cam->position.x = -(float)viewport_w * 0.5f + pos->position.x;
         //cam->position.y = -(float)viewport_h * 0.5f + pos->position.y;
         //cam->position.x = 0.0f;
         //cam->position.y = 0.0f;
         cam->prev_position.x = cam->position.x;
         cam->prev_position.y = cam->position.y;
         cam->win_aabb.size.x = aabb->aabb.size.x * win_scale;
         cam->win_aabb.size.y = aabb->aabb.size.y * win_scale;
         cam->win_aabb.position.x = pos->position.x + aabb->x_offset - (cam->win_aabb.size.x - aabb->aabb.size.x) * 0.5f;
         cam->win_aabb.position.y = pos->position.y + aabb->y_offset - (cam->win_aabb.size.y - aabb->aabb.size.y) * 0.5f;
         cam->target = aabb;

         EIR_KER_LOG_MESSAGE("camera info:");
         EIR_KER_LOG_MESSAGE("position (%f; %f)", cam->position.x, cam->position.y);
         EIR_KER_LOG_MESSAGE("prev position (%f; %f)", cam->prev_position.x, cam->prev_position.y);
      }
      else
      {
         EIR_KER_LOG_ERROR("TARGET ENTITY MUST HAVE AABB AND POSITION COMPONENTS");
      }
   }
}

void eir_gme_set_active_world(eir_gme_env_t * env, eir_gme_world_t * world)
{
   if (env && world)
   {
      env->curr_world = world;
   }
}
