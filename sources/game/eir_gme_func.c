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
      EIR_KER_INIT_ARRAY(world->entities);
      EIR_KER_INIT_ARRAY(world->positions);
      EIR_KER_INIT_ARRAY(world->sizes);
      EIR_KER_INIT_ARRAY(world->sprite_ref_handles);
      EIR_KER_INIT_ARRAY(world->colors);
      EIR_KER_INIT_ARRAY(world->motion_params);
      EIR_KER_INIT_ARRAY(world->aabbs);
      EIR_KER_INIT_ARRAY(world->cameras);
   }
}

static void eir_gme_release_world(eir_gme_world_t * world)
{
   if (world)
   {
      EIR_KER_FREE_ARRAY(world->entities);
      EIR_KER_FREE_ARRAY(world->positions);
      EIR_KER_FREE_ARRAY(world->sizes);
      EIR_KER_FREE_ARRAY(world->sprite_ref_handles);
      EIR_KER_FREE_ARRAY(world->colors);
      EIR_KER_FREE_ARRAY(world->motion_params);
      EIR_KER_FREE_ARRAY(world->aabbs);
      EIR_KER_FREE_ARRAY(world->cameras);
   }
}

static eir_gme_env_t * eir_gme_get_gme_env(eir_env_t * env)
{
   eir_gme_env_t * gme_env = 0;
   
   if (env)
   {
      gme_env = &(((eir_ker_env_t *)env->private)->gme_env);
   }
   return gme_env;
}

static void eir_gme_init_entity(eir_gme_entity_t * entity)
{
   if (entity)
   {
      *entity = eir_gme_component_type_none;
   }
}

static void eir_gme_release_entity(eir_gme_entity_t * entity)
{
   eir_gme_init_entity(entity);
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

static void eir_gme_init_sprite_ref_handle(eir_handle_t * sprite_ref_handle)
{
   if (sprite_ref_handle)
   {
      *sprite_ref_handle = EIR_INVALID_HANDLE;
   }
}

static void eir_gme_release_sprite_ref_handle(eir_handle_t * sprite_ref_handle)
{
   eir_gme_init_sprite_ref_handle(sprite_ref_handle);
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

static void eir_gme_init_motion_param(eir_phy_motion_param_t * motion_param)
{
   if (motion_param)
   {
      motion_param->velocity.x = 0.0f;
      motion_param->velocity.y = 0.0f;
      motion_param->acceleration.x = 0.0f;
      motion_param->acceleration.y = 0.0f;
      motion_param->speed_factor = 1.0f;
      motion_param->friction_factor = 0.0f;
   }
}

static void eir_gme_release_motion_param(eir_phy_motion_param_t * motion_param)
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
   }
}

static void eir_gme_release_camera(eir_gme_camera_component_t * camera)
{
   eir_gme_init_camera(camera);
}

static eir_gme_world_t * eir_gme_get_world(eir_gme_env_t * env, eir_handle_t world_handle)
{
   eir_gme_world_t * world = 0;

   if (env)
   {
      EIR_KER_GET_ARRAY_ITEM(env->worlds, world_handle, world);
   }
   return world;
}

static void eir_gme_init_button_state(eir_button_state_t * button_state)
{
   if (button_state)
   {
      button_state->pressed = false;
   }
}

static void eir_gme_init_input_controller(eir_input_controller_t * input_controller, bool is_connected)
{
   if (input_controller)
   {
      input_controller->is_connected = is_connected;
      input_controller->is_analog = false;
      input_controller->left_stick_value_x = 0.0f;
      input_controller->left_stick_value_y = 0.0f;
      for (int button_index = 0; button_index < EIR_TOTAL_INPUT_BUTTON_COUNT; ++button_index)
      {
	 eir_gme_init_button_state(&input_controller->buttons[button_index]);
      }
   }
}

static void eir_gme_init_input_controller_buffer(
   eir_input_controller_buffer_t * controller_buffer,
   bool is_connected
   )
{
   if (controller_buffer)
   {
      for (int index = 0; index < EIR_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT; ++index)
      {
	 eir_gme_init_input_controller(&controller_buffer->controllers[index], is_connected);
      }
   }
}

static void eir_gme_init_all_input_controller_buffer(eir_gme_env_t * env)
{
   if (env)
   {
      for (int index = 0; index < EIR_TOTAL_INPUT_CONTROLLER; ++index)
      {
	 bool is_connected = (index == 0 || index <= eir_sys_get_pad_count());
	 eir_gme_init_input_controller_buffer(&env->input_controllers[index], is_connected);
      }
   }
}

/*******************************************
 * INTERNAL FUNCTIONS
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
      for (int entity_index = 0; entity_index < world->entities.used; ++entity_index)
      {
	 eir_gme_entity_t * entity = &world->entities.data[entity_index];

	 if (
	    ((*entity) & eir_gme_component_type_aabb)
	    && ((*entity) & eir_gme_component_type_position)
	    )
	 {
	    eir_gme_aabb_component_t * aabb = &world->aabbs.data[entity_index];
	    eir_gme_position_component_t * pos = &world->positions.data[entity_index];
	    eir_gme_size_component_t * size = 0;
	    eir_gme_camera_component_t * cam = 0;

	    aabb->x_offset += pos->initial.x;
	    aabb->y_offset += pos->initial.y;

	    if ((*entity) & eir_gme_component_type_camera)
	    {
	       cam = &world->cameras.data[entity_index];

	       float size_x = aabb->width * cam->win_scale;
	       float size_y = aabb->height * cam->win_scale;
	       float position_x = aabb->x_offset + (aabb->width - size_x) * 0.5f;
	       float position_y = aabb->y_offset + (aabb->height - size_y) * 0.5f;

	       cam->position.x = -aabb->x_offset - aabb->width * 0.5f + 400; // TODO: remove hard coded value. use screen width / 2 instead
	       cam->position.y = -aabb->y_offset - aabb->height * 0.5f + 300; // TODO: remove hard coded value. use screen height / 2 instead
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
      EIR_KER_LOG_MESSAGE("release game env");
      EIR_KER_FREE_ARRAY_BIS(env->worlds, eir_gme_release_world);
      env->curr_world = 0;
   }
}

/*******************************************
 * EXTERNAL FUNCTIONS
 *****************************************/

void eir_gme_set_max_world_count(eir_env_t * env, size_t max_count)
{
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);

   if (gme_env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gme_world_t,
	 gme_env->worlds,
	 max_count,
	 eir_gme_init_world
	 );
   }
}

eir_handle_t eir_gme_create_world(eir_env_t * env, size_t max_entity_count)
{
   eir_handle_t world_handle = EIR_INVALID_HANDLE;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = 0;

   if (gme_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(gme_env->worlds, world, world_handle);
   }
   if (world)
   {  
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gme_entity_t,
	 world->entities,
	 max_entity_count,
	 eir_gme_init_entity
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
	 eir_handle_t,
	 world->sprite_ref_handles,
	 max_entity_count,
	 eir_gme_init_sprite_ref_handle
	 );
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_gme_color_component_t,
	 world->colors,
	 max_entity_count,
	 eir_gme_init_color
	 );
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_phy_motion_param_t,
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
	 eir_gme_camera_component_t,
	 world->cameras,
	 max_entity_count,
	 eir_gme_init_camera
	 );
      world->curr_camera = 0;
   }
   return world_handle;
}

void eir_gme_set_curr_world(eir_env_t * env, eir_handle_t world_handle)
{
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);

   if (gme_env && world)
   {
      gme_env->curr_world = world;
   }
}

eir_handle_t eir_gme_create_world_entity(eir_env_t * env, eir_handle_t world_handle)
{
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_handle_t entity_handle = EIR_INVALID_HANDLE;

   if (world)
   {
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->entities, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->positions, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->sizes, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->sprite_ref_handles, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->colors, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->motion_params, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->aabbs, entity_handle);
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(world->cameras, entity_handle);
   }
   return entity_handle;
}

bool eir_gme_set_world_entity_position(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   int x,
   int y
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_entity_t * entity = 0;
   eir_gme_position_component_t * position = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->positions, entity_handle, position);
   }
   if (entity && position)
   {
      (*entity) |= eir_gme_component_type_position;
      position->initial.x = (float)x;
      position->initial.y = (float)y;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity_handle);
   }
   return result;
}

bool eir_gme_set_world_entity_size(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   int width,
   int height
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_entity_t * entity = 0;
   eir_gme_size_component_t * size = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->sizes, entity_handle, size);
   }
   if (entity && size)
   {
      (*entity) |= eir_gme_component_type_size;
      size->initial.x = (float)width;
      size->initial.y = (float)height;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity_handle);
   }
   return result;
}

bool eir_gme_set_world_entity_sprite_ref(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   eir_handle_t sprite_ref_handle
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_entity_t * entity = 0;
   eir_handle_t * sprite_ref_handle_ptr = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->sprite_ref_handles, entity_handle, sprite_ref_handle_ptr);
   }
   if (entity && sprite_ref_handle_ptr)
   {
      (*entity) |= eir_gme_component_type_position;
      (*entity) |= eir_gme_component_type_size;
      (*entity) |= eir_gme_component_type_sprite;
      (*sprite_ref_handle_ptr) = sprite_ref_handle;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity_handle);
   }
   return result;
}

bool eir_gme_set_world_entity_color(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float r,
   float g,
   float b,
   float a
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_entity_t * entity = 0;
   eir_gme_color_component_t * color = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->colors, entity_handle, color);
   }
   if (entity && color)
   {
      (*entity) |= eir_gme_component_type_color;
      color->initial.r = r;
      color->initial.g = g;
      color->initial.b = b;
      color->initial.a = a;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity_handle);
   }
   return result;
}

bool eir_gme_set_world_entity_acceleration(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float x_acceleration,
   float y_acceleration,
   float speed_factor,
   float friction_factor
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_entity_t * entity = 0;
   eir_phy_motion_param_t * motion_param = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->motion_params, entity_handle, motion_param);
   }
   if (entity && motion_param)
   {
      (*entity) |= eir_gme_component_type_motion_param;
      motion_param->acceleration.x = x_acceleration;
      motion_param->acceleration.y = y_acceleration;
      motion_param->speed_factor = speed_factor;
      motion_param->friction_factor = friction_factor;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity_handle);
   }
   return result;
}

bool eir_gme_set_world_entity_aabb(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float x_offset,
   float y_offset,
   float width,
   float height
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_entity_t * entity = 0;
   eir_gme_aabb_component_t * aabb = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->aabbs, entity_handle, aabb);
   }
   if (entity && aabb)
   {
      (*entity) |= eir_gme_component_type_aabb;
      aabb->x_offset = x_offset;
      aabb->y_offset = y_offset;
      aabb->width = width;
      aabb->height = height;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity_handle);
   }
   return result;
}

bool eir_gme_set_world_entity_camera(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float cam_win_scale
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_entity_t * entity = 0;
   eir_gme_camera_component_t * camera = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->cameras, entity_handle, camera);
   }
   if (entity && camera)
   {
      (*entity) |= eir_gme_component_type_camera;
      camera->win_scale = cam_win_scale;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or components in array", entity_handle);
   }
   return result;
}

bool eir_gme_set_world_entity_active_camera(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle
   )
{
   bool result = false;
   eir_gme_env_t * gme_env = eir_gme_get_gme_env(env);
   eir_gme_world_t * world = eir_gme_get_world(gme_env, world_handle);
   eir_gme_camera_component_t * camera = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->cameras, entity_handle, camera);
   }
   if (camera)
   {
      world->curr_camera = camera;
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find camera %d", entity_handle);
   }
   return result;
}
