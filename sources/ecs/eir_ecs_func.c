#include "../eir.h"
#include "../kernel/eir_env.h"
#include "eir_ecs_env.h"

static eir_ecs_env_t * eir_ecs_get_ecs_env(eir_env_t * env)
{
   eir_ecs_env_t * ecs_env = 0;

   if (env)
   {
      ecs_env = &(((eir_ker_env_t *)env->private)->ecs_env);
   }
   return ecs_env;
}

static void eir_ecs_init_entity(eir_ecs_entity_t * entity)
{
   if (entity)
   {
      *entity = eir_ecs_component_type_none;
   }
}

static void eir_ecs_release_entity(eir_ecs_entity_t * entity)
{
   eir_ecs_init_entity(entity);
}

static void eir_ecs_init_position(eir_mth_vec2_t * position)
{
   if (position)
   {
      position->x = 0.0f;
      position->y = 0.0f;
   }
}

static void eir_ecs_release_position(eir_mth_vec2_t * position)
{
   eir_ecs_init_position(position);
}

static void eir_ecs_init_size(eir_mth_vec2_t * size)
{
   if (size)
   {
      size->x = 0.0f;
      size->y = 0.0f;
   }
}

static void eir_ecs_release_size(eir_mth_vec2_t * size)
{
   eir_ecs_init_position(size);
}

static void eir_ecs_init_sprite_ref_handle(eir_handle_t * sprite_ref_handle)
{
   if (sprite_ref_handle)
   {
      *sprite_ref_handle = EIR_INVALID_HANDLE;
   }
}

static void eir_ecs_release_sprite_ref_handle(eir_handle_t * sprite_ref_handle)
{
   eir_ecs_init_sprite_ref_handle(sprite_ref_handle);
}

void eir_ecs_init_env(eir_ecs_env_t * ecs_env)
{
   if (ecs_env)
   {
      EIR_KER_INIT_ARRAY(ecs_env->entities);
      EIR_KER_INIT_ARRAY(ecs_env->positions);
      EIR_KER_INIT_ARRAY(ecs_env->sizes);
      EIR_KER_INIT_ARRAY(ecs_env->sprites_ref_handles);
   }
}

void eir_ecs_release_env(eir_ecs_env_t * ecs_env)
{
   if (ecs_env)
   {
      EIR_KER_FREE_ARRAY(ecs_env->entities);
      EIR_KER_FREE_ARRAY(ecs_env->positions);
      EIR_KER_FREE_ARRAY(ecs_env->sizes);
      EIR_KER_FREE_ARRAY(ecs_env->sprites_ref_handles);
   }
}

void eir_ecs_set_max_entity_count(eir_env_t * env, size_t max_count)
{
   eir_ecs_env_t * ecs_env = eir_ecs_get_ecs_env(env);

   if (ecs_env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_ecs_entity_t,
	 ecs_env->entities,
	 max_count,
	 eir_ecs_init_entity
	 );
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_mth_vec2_t,
	 ecs_env->positions,
	 max_count,
	 eir_ecs_init_position
	 );
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_mth_vec2_t,
	 ecs_env->sizes,
	 max_count,
	 eir_ecs_init_size
	 );
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_handle_t,
	 ecs_env->sprite_ref_handles,
	 max_count,
	 eir_ecs_init_sprite_ref_handle
	 );
   }
}

eir_handle_t eir_ecs_create_entity(eir_env_t * env)
{
   eir_handle_t entity_handle = EIR_INVALID_HANDLE;
   eir_ecs_env_t * ecs_env = eir_ecs_get_ecs_env(env);

   if (ecs_env)
   {
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(ecs_env->entities, entity_handle);
   }
   return entity_handle;
}

bool eir_ecs_set_entity_position(eir_env_t * env, eir_handle_t entity_handle, int x, int y)
{
   bool result = false;
   eir_ecs_entity_t * entity;
   eir_ecs_env_t * ecs_env = eir_ecs_get_ecs_env(env);

   if (ecs_env)
   {
      EIR_KER_GET_ARRAY_ITEM(ecs_env->entities, entity, entity_handle);
   }
   return result;
}

bool eir_ecs_set_entity_size(eir_env_t * env, int width, int height)
{
}

bool eir_ecs_set_entity_sprite(eir_env_t * env, eir_handle_t sprite_ref_handle)
{
}
