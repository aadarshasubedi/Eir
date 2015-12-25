#include "eir_gme_func.h"
#include "../kernel/eir_ker_env.h"
#include "../kernel/eir_log.h"
//#include "../physics/eir_motion_func.h"

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

static void eir_gme_init_position(eir_mth_vec2_t * position)
{
   if (position)
   {
      position->x = 0.0f;
      position->y = 0.0f;
   }
}

static void eir_gme_release_position(eir_mth_vec2_t * position)
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
   eir_gme_init_position(size);
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
	 eir_mth_vec2_t,
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
	 eir_handle_t,
	 world->colors,
	 max_entity_count,
	 eir_gme_init_color
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
   eir_mth_vec2_t * position = 0;

   if (world)
   {
      EIR_KER_GET_ARRAY_ITEM(world->entities, entity_handle, entity);
      EIR_KER_GET_ARRAY_ITEM(world->positions, entity_handle, position);
   }
   if (entity && position)
   {
      (*entity) |= eir_gme_component_type_position;
      position->x = (float)x;
      position->y = (float)y;
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
      EIR_KER_GET_ARRAY_ITEM(world->colors, entity_handle,color);
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

/*
void eir_gme_proceed_player_move(
   eir_gme_player_state_t * player_state,
   eir_mth_vec2_t * velocity,
   double elapsed_time)
{
   if (player_state && velocity)
   {
      player_state->motion_param.velocity.x = velocity->x;
      player_state->motion_param.velocity.y = velocity->y;

      eir_mth_vec2_t new_position;
      eir_mth_vec2_t new_velocity;

      eir_phy_proceed_euler_integration(
	 &player_state->position,
	 &player_state->motion_param,
	 elapsed_time,
	 &new_position,
	 &new_velocity
	 );
      player_state->position.x = new_position.x;
      player_state->position.y = new_position.y;
   }
}
*/

/*

static void eir_proceed_player_move(
   eir_gme_player_state_t * player_state,
   eir_sys_env_t * sys_env,
   double elapsed_time
   )
{
   if (!player_state || !sys_env)
   {
      return;
   }

   eir_mth_vec2_t velocity;

   velocity.x = sys_env->joystick.x_axis_value;
   velocity.y = sys_env->joystick.y_axis_value;
   // TODO: check keyboard too if player 1 use keyboard instead of pad
   
   eir_gme_proceed_player_move(player_state, &velocity, elapsed_time);
}

eir_proceed_player_move(&gme_env->player_1_state, sys_env, sys_env->timer.elapsed_time);

 */
