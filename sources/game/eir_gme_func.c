#include "eir_gme_func.h"
#include "../kernel/eir_ker_env.h"
#include "../kernel/eir_log.h"
#include "../system/eir_joystick_func.h"
#include "../maths/eir_mth_func.h"

/*******************************************
 * LOCAL FUNCTIONS
 *****************************************/

static void eir_gme_init_world(eir_gme_world_t * world)
{
   if (world)
   {
      EIR_KER_INIT_ARRAY(world->entities_flags);
      EIR_KER_INIT_ARRAY(world->positions);
      EIR_KER_INIT_ARRAY(world->sizes);
      EIR_KER_INIT_ARRAY(world->sprite_refs);
      EIR_KER_INIT_ARRAY(world->colors);
      EIR_KER_INIT_ARRAY(world->motion_params);
      EIR_KER_INIT_ARRAY(world->aabbs);
      EIR_KER_INIT_ARRAY(world->cameras);
      EIR_KER_INIT_ARRAY(world->physics);
		EIR_KER_INIT_ARRAY(world->directions);
   }
}

static void eir_gme_release_world(eir_gme_world_t * world)
{
   if (world)
   {
      EIR_KER_FREE_ARRAY(world->entities_flags);
      EIR_KER_FREE_ARRAY(world->positions);
      EIR_KER_FREE_ARRAY(world->sizes);
      EIR_KER_FREE_ARRAY(world->sprite_refs);
      EIR_KER_FREE_ARRAY(world->colors);
      EIR_KER_FREE_ARRAY(world->motion_params);
      EIR_KER_FREE_ARRAY(world->aabbs);
      EIR_KER_FREE_ARRAY(world->cameras);
      EIR_KER_FREE_ARRAY(world->physics);
		EIR_KER_FREE_ARRAY(world->directions);
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
      position->initial.x = 0.0f;
      position->initial.y = 0.0f;
      position->current = 0;
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
      size->initial.x = 0.0f;
      size->initial.y = 0.0f;
      size->current = 0;
   }
}

static void eir_gme_release_size(eir_gme_size_component_t * size)
{
   eir_gme_init_size(size);
}

static void eir_gme_init_sprite_ref(eir_gme_sprite_ref_component_t * sprite_ref)
{
   if (sprite_ref)
   {
      sprite_ref->ptr = 0;
   }
}

static void eir_gme_release_sprite_ref(eir_gme_sprite_ref_component_t * sprite_ref)
{
   eir_gme_init_sprite_ref(sprite_ref);
}

static void eir_gme_init_color(eir_gme_color_component_t * color)
{
   if (color)
   {
      color->initial.r = 1.0f;
      color->initial.g = 1.0f;
      color->initial.b = 1.0f;
      color->initial.a = 1.0f;
      color->current = 0;
   }
}

static void eir_gme_release_color(eir_gme_color_component_t * color)
{
   eir_gme_init_color(color);
}

static void eir_gme_init_motion_param(eir_gme_motion_param_component_t * motion_param)
{
   if (motion_param)
   {
      motion_param->data.velocity.x = 0.0f;
      motion_param->data.velocity.y = 0.0f;
      motion_param->data.acceleration.x = 0.0f;
      motion_param->data.acceleration.y = 0.0f;
      motion_param->data.speed_factor = 1.0f;
      motion_param->data.friction_factor = 0.0f;
   }
}

static void eir_gme_release_motion_param(eir_gme_motion_param_component_t * motion_param)
{
   eir_gme_init_motion_param(motion_param);
}

static void eir_gme_init_aabb(eir_gme_aabb_component_t * aabb)
{
   if (aabb)
   {
      aabb->x_offset = 0.0f;
      aabb->y_offset = 0.0f;
      aabb->width = 0.0f;
      aabb->height = 0.0f;
      aabb->rect = 0;
   }
}

static void eir_gme_release_aabb(eir_gme_aabb_component_t * aabb)
{
   eir_gme_init_aabb(aabb);
}

static void eir_gme_init_camera(eir_gme_camera_component_t * camera)
{
   if (camera)
   {
      eir_mth_set_vec2(&camera->position, 0.0f, 0.0f);
      eir_mth_set_vec2(&camera->prev_position, 0.0f, 0.0f);
      camera->target = 0;
      camera->win_rect = 0;
      camera->win_scale = 1.0f;
      camera->viewport_w = 0;
      camera->viewport_h = 0;
   }
}

static void eir_gme_release_camera(eir_gme_camera_component_t * camera)
{
   eir_gme_init_camera(camera);
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
		direction->value = EIR_GME_DIRECTION_UNKNOWN;
	}
}

static void eir_gme_release_direction(eir_gme_direction_component_t * direction)
{
	eir_gme_init_direction(direction);
}

static void eir_gme_init_button_state(eir_gme_button_state_t * button_state)
{
   if (button_state)
   {
      button_state->pressed = false;
   }
}

static void eir_gme_init_input_controller(eir_gme_input_controller_t * input_controller, bool is_connected)
{
   if (input_controller)
   {
      input_controller->is_connected = is_connected;
      input_controller->is_analog = false;
      input_controller->left_stick_value_x = 0.0f;
      input_controller->left_stick_value_y = 0.0f;
      for (int button_index = 0; button_index < EIR_GME_TOTAL_INPUT_BUTTON_COUNT; ++button_index)
      {
	 eir_gme_init_button_state(&input_controller->buttons[button_index]);
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
      for (int index = 0; index < EIR_GME_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT; ++index)
      {
	 eir_gme_init_input_controller(&controller_buffer->controllers[index], is_connected);
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
	 eir_gme_init_input_controller_buffer(&env->input_controllers[index], is_connected);
      }
   }
}

/*******************************************
 * GLOBAL FUNCTIONS
 *****************************************/

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

void eir_gme_world_entity_update_linked_components(eir_gme_world_t * world)
{
   if (world)
   {
      for (int entity_index = 0; entity_index < world->entities_flags.used; ++entity_index)
      {
	 eir_gme_entity_flags_t * entity_flags = &world->entities_flags.data[entity_index];

	 if (
	    ((*entity_flags) & eir_gme_component_type_aabb)
	    && ((*entity_flags) & eir_gme_component_type_position)
	    )
	 {
	    eir_gme_aabb_component_t * aabb = &world->aabbs.data[entity_index];
	    eir_gme_position_component_t * pos = &world->positions.data[entity_index];
	    eir_gme_size_component_t * size = 0;
	    eir_gme_camera_component_t * cam = 0;

	    aabb->x_offset += pos->initial.x;
	    aabb->y_offset += pos->initial.y;

	    if ((*entity_flags) & eir_gme_component_type_camera)
	    {
	       cam = &world->cameras.data[entity_index];

	       float size_x = aabb->width * cam->win_scale;
	       float size_y = aabb->height * cam->win_scale;
	       float position_x = aabb->x_offset + (aabb->width - size_x) * 0.5f;
	       float position_y = aabb->y_offset + (aabb->height - size_y) * 0.5f;

	       cam->position.x = -aabb->x_offset - aabb->width * 0.5f + (float)cam->viewport_w * 0.5f;
	       cam->position.y = -aabb->y_offset - aabb->height * 0.5f + (float)cam->viewport_h * 0.5f;
	       cam->prev_position.x = aabb->x_offset;
	       cam->prev_position.y = aabb->y_offset;
	       cam->target = aabb;
	    }
	 }
      }
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
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_world_t, env->worlds, max_count, eir_gme_init_world);
   }
}

eir_gme_world_t * eir_gme_create_world(eir_gme_env_t * env, size_t max_entity_count)
{
   eir_gme_world_t * world = 0;

   if (env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->worlds, world);
   }
   if (world)
   {  
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_entity_flags_t, world->entities_flags, max_entity_count, eir_gme_init_entity_flags);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_position_component_t, world->positions, max_entity_count, eir_gme_init_position);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_size_component_t, world->sizes, max_entity_count, eir_gme_init_size);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_sprite_ref_component_t, world->sprite_refs, max_entity_count, eir_gme_init_sprite_ref);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_color_component_t, world->colors, max_entity_count, eir_gme_init_color);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_motion_param_component_t, world->motion_params, max_entity_count, eir_gme_init_motion_param);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_aabb_component_t, world->aabbs, max_entity_count, eir_gme_init_aabb);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_camera_component_t, world->cameras, max_entity_count, eir_gme_init_camera);
      EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_physic_component_t, world->physics, max_entity_count, eir_gme_init_physic);
		EIR_KER_ALLOCATE_ARRAY_BIS(eir_gme_direction_component_t, world->directions, max_entity_count, eir_gme_init_direction);
      world->curr_camera = 0;
   }
   return world;
}

eir_gme_entity_t eir_gme_create_world_entity(eir_gme_world_t * world)
{
   eir_gme_entity_t entity_handle = EIR_GME_INVALID_ENTITY;

   if (world)
   {
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->entities_flags, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->positions, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->sizes, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->sprite_refs, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->colors, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->motion_params, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->aabbs, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->cameras, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->physics, entity_handle);
		EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->directions, entity_handle);
   }
   return entity_handle;
}

eir_gme_position_component_t * eir_gme_set_entity_position(eir_gme_world_t * world, eir_gme_entity_t entity, int x, int y)
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_position_component_t * pos_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->positions, entity, pos_component);
   }
   if (entity_flags && pos_component)
   {
      (*entity_flags) |= eir_gme_component_type_position;
      pos_component->initial.x = (float)x;
      pos_component->initial.y = (float)y;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
   return pos_component;
}

eir_gme_size_component_t * eir_gme_set_entity_size(eir_gme_world_t * world, eir_gme_entity_t entity, int width, int height)
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
      size_component->initial.x = (float)width;
      size_component->initial.y = (float)height;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
   return size_component;
}

eir_gme_sprite_ref_component_t * eir_gme_set_entity_sprite_ref(eir_gme_world_t * world, eir_gme_entity_t entity, eir_gfx_sprite_ref_t * sprite_ref)
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_sprite_ref_component_t * sprite_ref_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->sprite_refs, entity, sprite_ref_component);
   }
   if (entity_flags && sprite_ref_component)
   {
      (*entity_flags) |= eir_gme_component_type_position;
      (*entity_flags) |= eir_gme_component_type_size;
      (*entity_flags) |= eir_gme_component_type_sprite;
      sprite_ref_component->ptr = sprite_ref;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
   return sprite_ref_component;
}

eir_gme_color_component_t * eir_gme_set_entity_color(eir_gme_world_t * world, eir_gme_entity_t entity, float r, float g, float b, float a)
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
      color_component->initial.r = r;
      color_component->initial.g = g;
      color_component->initial.b = b;
      color_component->initial.a = a;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
   return color_component;
}

eir_gme_motion_param_component_t * eir_gme_set_entity_acceleration(eir_gme_world_t * world, eir_gme_entity_t entity, float ax, float ay, float speed, float friction)
{
   eir_gme_entity_flags_t * entity_flags = 0;
   eir_gme_motion_param_component_t * motion_param_component = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities_flags, entity, entity_flags);
      EIR_KER_GET_ARRAY_ITEM(world->motion_params, entity, motion_param_component);
   }
   if (entity_flags && motion_param_component)
   {
      (*entity_flags) |= eir_gme_component_type_position;
      (*entity_flags) |= eir_gme_component_type_motion_param;
      motion_param_component->data.acceleration.x = ax;
      motion_param_component->data.acceleration.y = ay;
      motion_param_component->data.speed_factor = speed;
      motion_param_component->data.friction_factor = friction;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
   return motion_param_component;
}

eir_gme_aabb_component_t * eir_gme_set_entity_aabb(eir_gme_world_t * world, eir_gme_entity_t entity, float x, float y, float width, float height)
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
      (*entity_flags) |= eir_gme_component_type_position;
      (*entity_flags) |= eir_gme_component_type_aabb;
      aabb_component->x_offset = x;
      aabb_component->y_offset = y;
      aabb_component->width = width;
      aabb_component->height = height;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity);
   }
   return aabb_component;
}

eir_gme_camera_component_t * eir_gme_set_entity_camera(eir_gme_world_t * world, eir_gme_entity_t entity, float win_scale, int viewport_w, int viewport_h)
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
      (*entity_flags) |= eir_gme_component_type_position;
      (*entity_flags) |= eir_gme_component_type_camera;
      camera_component->win_scale = win_scale;
      camera_component->viewport_w = viewport_w;
      camera_component->viewport_h = viewport_h;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity);
   }
   return camera_component;
}

eir_gme_physic_component_t * eir_gme_set_entity_physic(eir_gme_world_t * world, eir_gme_entity_t entity, float weight)
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
   return physic_component;
}

eir_gme_direction_component_t * eir_gme_set_entity_direction(eir_gme_world_t * world, eir_gme_entity_t entity, eir_gme_direction_t direction)
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
		direction_component->value = direction;
	}
	else
	{
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity);
	}
	return direction_component;
}

void eir_gme_set_active_camera(eir_gme_world_t * world, eir_gme_entity_t entity)
{
   eir_gme_camera_component_t * camera = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->cameras, entity, camera);
   }
   if (camera)
   {
      world->curr_camera = camera;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find camera from entity %d", entity);
   }
}

void eir_gme_set_active_world(eir_gme_env_t * env, eir_gme_world_t * world)
{
   if (env && world)
   {
      env->curr_world = world;
   }
}
