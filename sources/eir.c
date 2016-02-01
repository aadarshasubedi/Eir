#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "eir.h"

#include "maths/eir_vector.h"

#include "system/eir_sys_env.h"
#include "system/eir_timer_func.h"
#include "system/eir_file_system.h"
#include "system/eir_memory.h"
#include "system/eir_joystick_func.h"
#include "system/eir_win_api_func.h"

#include "graphics/eir_sprite.h"
#include "graphics/eir_gfx_types.h"
#include "graphics/eir_gfx_defines.h"
#include "graphics/eir_sprite_batch.h"
#include "graphics/eir_gfx_env.h"
#include "graphics/eir_gfx_api_func.h"
#include "graphics/eir_gfx_func.h"

#include "sound/eir_snd_api_func.h"
#include "sound/eir_snd_env.h"

#include "game/eir_gme_env.h"
#include "game/eir_gme_func.h"

#include "kernel/eir_ker_env.h"

#include "fsm/eir_fsm_func.h"

#include "physics/eir_motion_func.h"

static void eir_init_all_api(eir_ker_env_t * env, int width, int height)
{
   eir_sys_win_api_init();
   eir_sys_win_api_create_window(&env->sys_env, width, height);
   eir_gfx_api_init();
   eir_snd_api_init();
   eir_gfx_api_load_sprite_shaders(&env->gfx_env);
   eir_gfx_api_load_text_shaders(&env->gfx_env);
   eir_gfx_api_load_rect_shaders(&env->gfx_env);
}

static void eir_stop(eir_ker_env_t * env)
{
   eir_gfx_api_unload_rect_shaders(&env->gfx_env);
   eir_gfx_api_unload_text_shaders(&env->gfx_env);
   eir_gfx_api_unload_sprite_shaders(&env->gfx_env);
   eir_snd_api_release();
   eir_gfx_api_release();
   eir_sys_win_api_destroy_window(&env->sys_env);
   eir_sys_win_api_release();
}

static void eir_check_allocate_and_free_func()
{
   if (!eir_sys_allocate)
   {
      eir_sys_allocate = eir_sys_default_allocate;
   }
   if (!eir_sys_free)
   {
      eir_sys_free = eir_sys_default_free;
   }
}

static void eir_render_frame_rate(eir_gfx_env_t * gfx_env, float elapsed_time, eir_handle_t text_handle)
{
   char c[32];
   sprintf(c, "frame rate: %1.3f", elapsed_time);
   eir_gfx_update_text(gfx_env, text_handle, c);
}

void eir_set_allocate_func(eir_allocate_t allocate_func)
{
   eir_sys_allocate = allocate_func;
}

void eir_set_free_func(eir_free_t free_func)
{
   eir_sys_free = free_func;
}

eir_env_t * eir_create_env(int width, int height)
{
   eir_check_allocate_and_free_func();

   eir_env_t * private_env = eir_sys_allocate(sizeof(eir_env_t), 1);
   eir_ker_env_t * env = 0;

   if (private_env)
   {
      private_env->private = eir_sys_allocate(sizeof(eir_ker_env_t), 1);
      env = (eir_ker_env_t *)(private_env->private);
   }
   if (env)
   {
      eir_init_all_api(env, width, height);
      eir_gfx_init_env(&env->gfx_env, width, height);
      eir_snd_init_env(&env->snd_env);
      eir_fsm_init_env(&env->fsm_env);
      eir_sys_init_env(&env->sys_env);
      eir_gme_init_env(&env->gme_env);
   }
   return private_env;
}

eir_input_controller_buffer_t * eir_get_input_controller_buffer(eir_env_t * env, int controller_index)
{
   eir_input_controller_buffer_t * input_controller_buffer = 0;

   if (env && controller_index >= 0 && controller_index < EIR_TOTAL_INPUT_CONTROLLER)
   {
      eir_ker_env_t * all_env = (eir_ker_env_t *)(env->private);
      input_controller_buffer = &all_env->gme_env.input_controllers[controller_index];
   }
   return input_controller_buffer;
}

double eir_get_current_time(eir_env_t * env)
{
   double result = 0.0;

   if (env && env->private)
   {
      eir_sys_env_t * sys_env = &((eir_ker_env_t *)(env->private))->sys_env;
      result = sys_env->timer.last_time;
   }
   return result;
}

void eir_run(eir_env_t * env)
{
   if (!env)
   {
      return;
   }
   if (!env->private)
   {
      return;
   }

   eir_ker_env_t * all_env = (eir_ker_env_t *)(env->private);

   if (!all_env)
   {
      return;
   }

   eir_gfx_env_t * gfx_env = &all_env->gfx_env;
   eir_sys_env_t * sys_env = &all_env->sys_env;
   eir_snd_env_t * snd_env = &all_env->snd_env;
   eir_gme_env_t * gme_env = &all_env->gme_env;
   eir_fsm_env_t * fsm_env = &all_env->fsm_env;

   if (!gfx_env || !sys_env || !snd_env || !gme_env)
   {
      return;
   }

   eir_gfx_set_text_capacity(gfx_env, 1);
   eir_gfx_set_rect_capacity(gfx_env, 10);
   //eir_snd_set_sound_capacity(snd_env, 2);

#ifdef EIR_DEBUG
   eir_mth_vec2_t position;
   eir_gfx_color_t color;
   eir_handle_t frame_rate_text_handle;

   position.x = 0.0f;
   position.y = 0.0f;
   color.r = 0.0f;
   color.g = 1.0f;
   color.b = 0.0f;
   color.a = 0.7f;
   frame_rate_text_handle = eir_gfx_add_text(gfx_env, "DEBUG TEST TEXT", &position, 32.0f, &color);
#endif

   float time_per_frame = 1.0f / 60.0f;
   float time_since_last_update = 0.0f;

   eir_sys_start_timer(&sys_env->timer);
   eir_fsm_run_state_machine(&fsm_env->state_machines.data[0]); // TODO: use returned pointer of create fsm function
   eir_gme_world_entity_update_linked_components(gme_env->curr_world);
   eir_gfx_generate_all_batches(gfx_env, gme_env->curr_world);
   for (;;)
   {
      eir_sys_update_timer(&sys_env->timer);
      time_since_last_update += sys_env->timer.elapsed_time;
      while (time_since_last_update > time_per_frame)
      {
	 time_since_last_update -= time_per_frame;
	 if (!eir_sys_win_api_poll_all_events(gme_env, sys_env))
	 {
	    return;
	 }

	 // UPDATE ALL SYSTEMS HERE EXCEPT RENDERING AND TIMER
	 eir_fsm_update_state_machine(&fsm_env->state_machines.data[0]); // TODO: use returned pointer of create fsm function
	 eir_phy_proceed_motion_entity_update(gme_env->curr_world, time_per_frame);
	 eir_gme_proceed_camera_system_update(gme_env->curr_world);

         if (gme_env->curr_world && gme_env->curr_world->curr_camera)
         {
            eir_gfx_update_camera_view(gfx_env, &gme_env->curr_world->curr_camera->position);
         }
	 eir_gfx_force_update_all_batches(gfx_env); // TODO: update only real modified batch
      }

#ifdef EIR_DEBUG
      eir_render_frame_rate(gfx_env, sys_env->timer.elapsed_time, frame_rate_text_handle);
#endif
      eir_gfx_api_set_clear_color();
      eir_gfx_api_clear_buffer();
      eir_gfx_render_all_batches(gfx_env);
      eir_sys_win_api_swap_buffer(sys_env);
   }
}

void eir_destroy_env(eir_env_t * env)
{
   EIR_KER_LOG_MESSAGE("destroy all env");
   if (env)
   {
      if (env->private)
      {
	 eir_ker_env_t * all_env = (eir_ker_env_t *)(env->private);

	 eir_stop(all_env);
	 eir_gfx_release_env(&all_env->gfx_env);
	 eir_snd_release_env(&all_env->snd_env);
	 eir_fsm_release_env(&all_env->fsm_env);
	 eir_sys_release_env(&all_env->sys_env);
	 eir_gme_release_env(&all_env->gme_env);
	 eir_sys_free(env->private);
	 env->private = 0;
      }
      eir_sys_free(env);
      env = 0;
   }
}

void eir_display_mem_leaks()
{
#ifdef EIR_DEBUG
   EIR_SYS_LOG_ALLOCATED_ELEM;
#endif
}

#include <stdio.h>
#include "eir.h"

#define PLACE_HOLDER_IMAGE_PATH "../resources/images/placeholder_atlas.png"
#define PLAYER_FRICTION 10.0f
#define PLAYER_SPEED 1600.0f
#define PLAYER_GRAVITY 1.0f
#define PLAYER_JUMP_SPEED -2.0f

typedef struct
{
   eir_env_t * env;
   eir_handle_t world_handle;
   eir_handle_t entity_handle;
   eir_input_controller_buffer_t * keyboard_buffer;
   eir_input_controller_buffer_t * pad_buffer;
} player_t;

static bool validate_idle_state(void * user_data)
{
   bool result = false;
   
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      if (!player->pad_buffer->controllers[1].is_analog)
      {
	 if (
	    player->keyboard_buffer
	    && player->pad_buffer
	    && !player->pad_buffer->controllers[1].is_connected
	    && !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    && !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    //&& !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    //&& !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 {
	    result = true;
	 }
	 else if (
	    player->pad_buffer
	    && player->pad_buffer->controllers[1].is_connected
	    && !player->pad_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    && !player->pad_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    //&& !player->pad_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    //&& !player->pad_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 {
	    result = true;
	 }
      }
   }
   return result;
}

static bool validate_move_state(void * user_data)
{
   bool result = false;

   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      if (
	 player->keyboard_buffer
	 && player->pad_buffer
	 && !player->pad_buffer->controllers[1].is_connected
	 && (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    //|| player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    //|| player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 )
      {
	 result = true;
      }
      else if (
	 player->pad_buffer
	 && player->pad_buffer->controllers[1].is_connected
	 && !player->pad_buffer->controllers[1].is_analog
	 && (
	    player->pad_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    //|| player->pad_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    //|| player->pad_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 )
      {
	 result = true;
      }
      else if (
	 player->pad_buffer
	 && player->pad_buffer->controllers[1].is_connected
	 && player->pad_buffer->controllers[1].is_analog
	 && (
	    player->pad_buffer->controllers[1].left_stick_value_x
	    //|| player->pad_buffer->controllers[1].left_stick_value_y
	    )
	 )
      {
	 result = true;
      }
   }
   return result;
}

static bool validate_jump_up_state(void * user_data)
{
   bool result = false;

   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      if (
            player->keyboard_buffer
            && player->keyboard_buffer->controllers[1].buttons[EIR_JUMP_BUTTON_INDEX].pressed
         )
      {
         result = true;
      }
   }
   return result;
}

static void update_idle_state(void * user_data)
{
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      eir_gme_set_world_entity_acceleration(
	 player->env,
	 player->world_handle,
	 player->entity_handle,
	 0.0f,
	 PLAYER_GRAVITY, 
	 PLAYER_SPEED,
	 PLAYER_FRICTION
	 );
   }
}

static void update_move_state(void * user_data)
{
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      float x_velocity = 0.0f;

      if (player->pad_buffer->controllers[1].is_analog)
      {
	 x_velocity = player->pad_buffer->controllers[1].left_stick_value_x;
	 //y_velocity = player->pad_buffer->controllers[1].left_stick_value_y;
      }
      else
      {
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    )
	 {
	    x_velocity = 1.0f;
	 }
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    )
	 {
	    x_velocity = -1.0f;
	 }
         /*
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    )
	 {
	    y_velocity = 1.0f;
	 }
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 {
	    y_velocity = -1.0f;
	 }
         */
      }

      /*
      if (eir_mth_abs(x_velocity) > 0.0f && eir_mth_abs(y_velocity) > 0.0f)
      {
	 x_velocity *= 0.707107f;
	 y_velocity *= 0.707107f;
      }
      */

      eir_gme_set_world_entity_acceleration(
	 player->env,
	 player->world_handle,
	 player->entity_handle,
	 x_velocity,
	 PLAYER_GRAVITY,
	 PLAYER_SPEED,
	 PLAYER_FRICTION
	 );
   }
}

static void update_jump_up_state(void * user_data)
{
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      eir_gme_set_world_entity_acceleration(
	 player->env,
	 player->world_handle,
	 player->entity_handle,
	 0.0f,
	 PLAYER_JUMP_SPEED, 
	 PLAYER_SPEED,
	 PLAYER_FRICTION
	 );
   }
}

int main()
{
   // CREATE ENV

   eir_env_t * env = 0;
   env = eir_create_env(800, 600);
   
   eir_ker_env_t * all_env = (eir_ker_env_t *)(env->private);
   eir_fsm_env_t * fsm_env = &all_env->fsm_env;

   // INIT GFX ITEMS CAPACITY

   eir_gfx_set_max_image_count(env, 1);
   eir_gfx_set_max_sprite_ref_count(env, 1);
   eir_gfx_set_max_texture_count(env, 1);

   // LOAD SPRITE

   eir_handle_t ph_atlas_image = eir_gfx_load_image(env, PLACE_HOLDER_IMAGE_PATH, false);
   eir_handle_t ph_texture = eir_gfx_create_texture(env, ph_atlas_image);
   eir_handle_t ph_sprite_ref = eir_gfx_create_sprite_ref(env, ph_texture, 0, 0, 64, 64);

   // INIT WORLD ENTITIES

   eir_gme_set_max_world_count(env, 1);

   eir_handle_t world = eir_gme_create_world(env, 10);
   
   eir_gme_set_curr_world(env, world);

   eir_handle_t entity = eir_gme_create_world_entity(env, world);

   eir_gme_set_world_entity_position(env, world, entity, 400, 300);
   eir_gme_set_world_entity_size(env, world, entity, 64, 64);
   eir_gme_set_world_entity_sprite_ref(env, world, entity, ph_sprite_ref);
   eir_gme_set_world_entity_color(env, world, entity, 0.0f, 1.0f, 0.0f, 0.5f);
   eir_gme_set_world_entity_acceleration(env, world, entity, 0.0f, 0.0f, PLAYER_SPEED, PLAYER_FRICTION);
   eir_gme_set_world_entity_aabb(env, world, entity, 0.0f, 0.0f, 64.0f, 64.0f);
   eir_gme_set_world_entity_camera(env, world, entity, 2.0f);
   eir_gme_set_world_entity_active_camera(env, world, entity);
   eir_gme_set_world_entity_physic(env, world, entity, 1.0f);
   
   eir_handle_t entity2 = eir_gme_create_world_entity(env, world);

   eir_gme_set_world_entity_position(env, world, entity2, 100, 100);
   eir_gme_set_world_entity_size(env, world, entity2, 64, 64);
   eir_gme_set_world_entity_sprite_ref(env, world, entity2, ph_sprite_ref);
   eir_gme_set_world_entity_color(env, world, entity2, 0.0f, 1.0f, 0.0f, 0.5f);
   eir_gme_set_world_entity_aabb(env, world, entity2, 0.0f, 0.0f, 128.0f, 128.0f);
   eir_gme_set_world_entity_physic(env, world, entity2, 1.0f);
   
   eir_handle_t ground = eir_gme_create_world_entity(env, world);

   eir_gme_set_world_entity_position(env, world, ground, 0, 500);
   eir_gme_set_world_entity_size(env, world, ground, 1800, 32);
   eir_gme_set_world_entity_sprite_ref(env, world, ground, ph_sprite_ref);
   eir_gme_set_world_entity_color(env, world, ground, 1.0f, 1.0f, 1.0f, 0.5f);
   eir_gme_set_world_entity_aabb(env, world, ground, 0.0f, 0.0f, 1800.0f, 32.0f);
   eir_gme_set_world_entity_physic(env, world, ground, 1.0f);

   // INIT PLAYER USER DATA

   player_t player;

   player.env = env;
   player.world_handle = world;
   player.entity_handle = entity;
   player.keyboard_buffer = eir_get_input_controller_buffer(env, 0);
   player.pad_buffer = eir_get_input_controller_buffer(env, 1);

   // INIT STATE MACHINE

   eir_fsm_set_max_state_machine_count(fsm_env, 1);

   eir_fsm_state_machine_t *    fsm             = eir_fsm_create_state_machine(fsm_env, 4, &player);
   eir_fsm_state_t *            idle_state      = eir_fsm_create_state(fsm);
   eir_fsm_state_t *            move_state      = eir_fsm_create_state(fsm);
   eir_fsm_state_t *            end_state       = eir_fsm_create_state(fsm);
   eir_fsm_state_t *            jump_up_state   = eir_fsm_create_state(fsm);

   eir_fsm_set_begin_state(fsm, idle_state);
   eir_fsm_set_end_state(fsm, end_state);
   
   eir_fsm_set_state_validate_func(idle_state, validate_idle_state);
   eir_fsm_set_state_validate_func(move_state, validate_move_state);
   eir_fsm_set_state_validate_func(jump_up_state, validate_jump_up_state);

   eir_fsm_set_state_update_func(idle_state, update_idle_state);
   eir_fsm_set_state_update_func(move_state, update_move_state);
   eir_fsm_set_state_update_func(jump_up_state, update_jump_up_state);

   eir_fsm_add_state_transition(idle_state, move_state);
   eir_fsm_add_state_transition(idle_state, jump_up_state);
   eir_fsm_add_state_transition( move_state, idle_state);
   eir_fsm_add_state_transition(move_state, jump_up_state);
   eir_fsm_add_state_transition(jump_up_state, idle_state);
   eir_fsm_add_state_transition(jump_up_state, move_state);

   // RUN EIR ENGINE

   eir_run(env);

   // DESTROY ENV

   eir_destroy_env(env);
   eir_display_mem_leaks();

   return 0;
}
