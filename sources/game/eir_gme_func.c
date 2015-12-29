#include "eir_gme_func.h"
#include "../kernel/eir_ker_env.h"
#include "../kernel/eir_log.h"

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

static void eir_gme_init_size(eir_mth_vec2_t * size)
{
   if (size)
   {
      size->x = 0.0f;
      size->y = 0.0f;
   }
}

static void eir_gme_release_size(eir_mth_vec2_t * size)
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

static void eir_gme_init_color(eir_gfx_color_t * color)
{
   if (color)
   {
      color->r = 1.0f;
      color->g = 1.0f;
      color->b = 1.0f;
      color->a = 1.0f;
   }
}

static void eir_gme_release_color(eir_gfx_color_t * color)
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
   }
}

static void eir_gme_release_motion_param(eir_phy_motion_param_t * motion_param)
{
   eir_gme_init_motion_param(motion_param);
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

/*******************************************
 * INTERNAL FUNCTIONS
 *****************************************/

void eir_gme_init_env(eir_gme_env_t * env)
{
   if (env)
   {
      EIR_KER_INIT_ARRAY(env->worlds);
      env->curr_world = 0;
   }
}

void eir_gme_release_env(eir_gme_env_t * env)
{
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
	 eir_mth_vec2_t,
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
	 eir_gfx_color_t,
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
   eir_mth_vec2_t * size = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->sizes, entity_handle, size);
   }
   if (entity && size)
   {
      (*entity) |= eir_gme_component_type_size;
      size->x = (float)width;
      size->y = (float)height;
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
   eir_gfx_color_t * color = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->colors, entity_handle, color);
   }
   if (entity && color)
   {
      (*entity) |= eir_gme_component_type_color;
      color->r = r;
      color->g = g;
      color->b = b;
      color->a = a;
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
   float speed_factor
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
      result = true;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot find entity %d or component in array", entity_handle);
   }
   return result;
}
