/* NOTES: eir WIP infos and list of tasks
 *
 * TASKS:
 *
 *		- re design object rendering passing by push and pull function to add renderable object :
 *			- add static batch type for all entity without state component (like background and other non modifiable and movable object)
 *			- add dynamic batch type for state component entity
 *			- add batch group (only logical) creation (by user)
 *			- add shorcut to display / hide group
 *			- add push to group function
 *			- add pull from group function (only available for dynamic object)
 *    - finish based melee attack implementation
 *		- increase epsilon direction value
 *		- do not load and create texture for each new text batch : init once and use it many times !
 *		- fix files indentation
 *    - increase performance by update only modified batch (currently all batch are updated)
 *    - add real texture font manager to stock texture size and cell size anywhere else than in the add_text function
 *		- change add_text return type and update_text parameter type to manipulate created text
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600; 

const float PLAYER_FRICTION = 10.0f;
const float PLAYER_SPEED = 1600.0f;

typedef struct
{
   eir_ker_env_t * env;
   eir_gme_world_t * owner;
   eir_gme_entity_t entity;
   eir_gme_input_controller_buffer_t * keyboard_buffer;
   eir_gme_input_controller_buffer_t * pad_buffer;
   eir_fsm_state_machine_t * fsm;
	eir_gme_direction_component_t * direction_component;
	eir_gme_motion_param_component_t * motion_param_component;
	eir_gme_based_melee_attack_component_t * based_melee_attack_component;
} player_t;

typedef struct
{
   player_t player;
} game_t;

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

static void eir_render_frame_rate(eir_gfx_env_t * gfx_env, float elapsed_time, int text_handle)
{
   char c[32];
   sprintf(c, "frame rate: %1.3f", elapsed_time);
   eir_gfx_update_text(gfx_env, text_handle, c);
}

static void eir_render_player_direction(eir_gfx_env_t * gfx_env, eir_gme_direction_t direction, int text_handle)
{
	if (direction == EIR_GME_DIRECTION_UP)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: up");
	}
	else if (direction == EIR_GME_DIRECTION_UP_RIGHT)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: up/right");
	}
	else if (direction == EIR_GME_DIRECTION_RIGHT)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: right");
	}
	else if (direction == EIR_GME_DIRECTION_BOTTOM_RIGHT)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: bottom/right");
	}
	else if (direction == EIR_GME_DIRECTION_BOTTOM)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: bottom");
	}
	else if (direction == EIR_GME_DIRECTION_BOTTOM_LEFT)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: bottom/left");
	}
	else if (direction == EIR_GME_DIRECTION_LEFT)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: left");
	}
	else if (direction == EIR_GME_DIRECTION_UP_LEFT)
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: up/left");
	}
	else 
	{
		eir_gfx_update_text(gfx_env, text_handle, "direction: unknown");
	}
}

static void eir_render_player_velocity(eir_gfx_env_t * gfx_env, float x, float y, int text_handle)
{
	char c [32];
	sprintf(c, "velocity: (%1.3f; %1.3f)", x, y);
	eir_gfx_update_text(gfx_env, text_handle, c);
}

static void eir_set_allocate_func(eir_allocate_t allocate_func)
{
   eir_sys_allocate = allocate_func;
}

static void eir_set_free_func(eir_free_t free_func)
{
   eir_sys_free = free_func;
}

static void eir_init_env(eir_ker_env_t * env, int width, int height)
{
   eir_check_allocate_and_free_func();
   if (env)
   {
      eir_init_all_api(env, width, height);
      eir_gfx_init_env(&env->gfx_env, width, height);
      eir_snd_init_env(&env->snd_env);
      eir_fsm_init_env(&env->fsm_env);
      eir_sys_init_env(&env->sys_env);
      eir_gme_init_env(&env->gme_env);
   }
}

static eir_gme_input_controller_buffer_t * eir_get_input_controller_buffer(eir_gme_env_t * env, int controller_index)
{
   eir_gme_input_controller_buffer_t * input_controller_buffer = 0;

   if (env && controller_index >= 0 && controller_index < EIR_GME_TOTAL_INPUT_CONTROLLER)
   {
      input_controller_buffer = &env->input_controllers[controller_index];
   }
   return input_controller_buffer;
}

static double eir_get_current_time(eir_sys_env_t * env)
{
   double result = 0.0;

   if (env)
   {
      result = env->timer.last_time;
   }
   return result;
}

static void eir_run(game_t * game)
{
   player_t * player = &game->player;
   eir_ker_env_t * env = player->env;
   eir_gfx_env_t * gfx_env = &env->gfx_env;
   eir_sys_env_t * sys_env = &env->sys_env;
   eir_snd_env_t * snd_env = &env->snd_env;
   eir_gme_env_t * gme_env = &env->gme_env;
   eir_fsm_env_t * fsm_env = &env->fsm_env;

   if (!gfx_env || !sys_env || !snd_env || !gme_env)
   {
      return;
   }

   eir_gfx_set_text_capacity(gfx_env, 3);
   eir_gfx_set_rect_capacity(gfx_env, 10);
   //eir_snd_set_sound_capacity(snd_env, 2);

#ifdef EIR_DEBUG
   eir_mth_vec2_t position;
   eir_gfx_color_t color;
   int frame_rate_text_handle;
	int player_info_text_handle;
	int player_velocity_text_handle;

   position.x = 0.0f;
   position.y = 0.0f;
   color.r = 0.0f;
   color.g = 1.0f;
   color.b = 0.0f;
   color.a = 0.7f;
   frame_rate_text_handle = eir_gfx_add_text(gfx_env, "DEBUG TEST TEXT", &position, 32.0f, &color);
   position.y = 40.0f;
	player_info_text_handle = eir_gfx_add_text(gfx_env, "DEBUG TEST TEXT", &position, 16.0f, &color);
	position.y = 60.0f;
	player_velocity_text_handle = eir_gfx_add_text(gfx_env, "DEBUG TEST TEXT", &position, 16.0f, &color);
#endif

   float time_per_frame = 1.0f / 60.0f;
   float time_since_last_update = 0.0f;

   eir_sys_start_timer(&sys_env->timer);
   eir_fsm_run_state_machine(player->fsm);
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
			eir_fsm_update_state_machine(player->fsm);
			eir_phy_proceed_motion_entity_update(gme_env->curr_world, time_per_frame);
			eir_gme_proceed_camera_system_update(gme_env->curr_world);

         if (gme_env->curr_world && gme_env->curr_world->curr_camera)
         {
            eir_gfx_update_camera_view(gfx_env, &gme_env->curr_world->curr_camera->position);
         }
			eir_gfx_force_update_all_batches(gfx_env);
      }

#ifdef EIR_DEBUG
      eir_render_frame_rate(gfx_env, sys_env->timer.elapsed_time, frame_rate_text_handle);
		eir_render_player_direction(gfx_env, player->direction_component->value, player_info_text_handle);
		eir_render_player_velocity(gfx_env, player->motion_param_component->data.velocity.x, player->motion_param_component->data.velocity.y, player_velocity_text_handle);
#endif
      eir_gfx_api_set_clear_color();
      eir_gfx_api_clear_buffer();
      eir_gfx_render_all_batches(gfx_env);
      eir_sys_win_api_swap_buffer(sys_env);
   }
}

static void eir_release_env(eir_ker_env_t * env)
{
   EIR_KER_LOG_MESSAGE("destroy all env");
   if (env)
   {
      eir_stop(env);
      eir_gfx_release_env(&env->gfx_env);
      eir_snd_release_env(&env->snd_env);
      eir_fsm_release_env(&env->fsm_env);
      eir_sys_release_env(&env->sys_env);
      eir_gme_release_env(&env->gme_env);
   }
}

static void eir_display_mem_leaks()
{
#ifdef EIR_DEBUG
   EIR_SYS_LOG_ALLOCATED_ELEM;
#endif
}

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
	    && !player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
	    && !player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
		 && !player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
	    && !player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
	    && !player->keyboard_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
	    )
	 {
	    result = true;
	 }
	 else if (
	    player->pad_buffer
	    && player->pad_buffer->controllers[1].is_connected
	    && !player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
	    && !player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
	    && !player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
	    && !player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
	    && !player->pad_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
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
	    player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
	    || player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
	    || player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
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
	    player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
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
	    || player->pad_buffer->controllers[1].left_stick_value_y
	    )
	 )
      {
	 result = true;
      }
   }
   return result;
}

static bool validate_prepare_based_melee_attack_state(void * user_data)
{
	bool result = false;

	if (user_data)
	{
		player_t * player = (player_t *)user_data;

		if (
			player->keyboard_buffer
			&& player->pad_buffer
			&& !player->pad_buffer->controllers[1].is_connected
			&& player->keyboard_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
			)
		{
			result = true;
		}
		else if (
			player->pad_buffer
			&& player->pad_buffer->controllers[1].is_connected
			&& player->pad_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
			)
		{
			result = true;
		}
	}
	return result;
}

static bool validate_release_based_melee_attack_state(void * user_data)
{
	bool result = false;

	if (user_data)
	{
		player_t * player = (player_t *)user_data;

		if (
			player->keyboard_buffer
			&& player->pad_buffer
			&& !player->pad_buffer->controllers[1].is_connected
			&& !player->keyboard_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
			)
		{
			result = true;
		}
		else if (
			player->pad_buffer
			&& player->pad_buffer->controllers[1].is_connected
			&& !player->pad_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
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

      eir_gme_set_entity_acceleration(
			player->owner,
			player->entity,
			0.0f,
			0.0f, 
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
      float y_velocity = 0.0f;

      if (player->pad_buffer->controllers[1].is_analog)
      {
			x_velocity = player->pad_buffer->controllers[1].left_stick_value_x;
			y_velocity = player->pad_buffer->controllers[1].left_stick_value_y;
      }
      else
      {
			if (
				player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
				|| player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
				)
			{
				x_velocity = 1.0f;
			}
			if (
				player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
				|| player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
				)
			{
				x_velocity = -1.0f;
			}
			if (
				player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
				|| player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
				)
			{
				y_velocity = 1.0f;
			}
			if (
				player->keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
				|| player->pad_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
				)
			{
				y_velocity = -1.0f;
			}
      }

      if (eir_mth_abs(x_velocity) > 0.0f && eir_mth_abs(y_velocity) > 0.0f)
      {
			//x_velocity *= 0.707107f;
			//y_velocity *= 0.707107f;
      }

      eir_gme_set_entity_acceleration(
			player->owner,
			player->entity,
			x_velocity,
			y_velocity,
			PLAYER_SPEED,
			PLAYER_FRICTION
		);
   }
}

static void update_prepare_based_melee_attack_state(void * user_data)
{
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      eir_gme_set_entity_acceleration(
			player->owner,
			player->entity,
			0.0f,
			0.0f, 
			PLAYER_SPEED,
			PLAYER_FRICTION
			);

		// TODO: store current time in player for global cooldown and channel energy
   }
}

static void update_release_based_melee_attack_state(void * user_data)
{
	if (user_data)
	{
		player_t * player = (player_t *)user_data;

		// TODO: update AABB values using entity current position and orientation



		// TODO: display AABB
	}
}

int main()
{
   // INIT ENV

   eir_ker_env_t env;
   eir_init_env(&env, WINDOW_WIDTH, WINDOW_HEIGHT);
   
   eir_fsm_env_t * fsm_env = &env.fsm_env;
   eir_gme_env_t * gme_env = &env.gme_env;
   eir_gfx_env_t * gfx_env = &env.gfx_env;

	game_t game;

   // INIT GFX ITEMS CAPACITY

   eir_gfx_set_image_capacity(gfx_env, 1);
   eir_gfx_set_sprite_ref_capacity(gfx_env, 1);
   eir_gfx_set_texture_capacity(gfx_env, 1);

   // LOAD SPRITE

   eir_gfx_image_t * ph_atlas_image = eir_gfx_load_image(gfx_env, PLACE_HOLDER_IMAGE_PATH, false);
   eir_gfx_texture_t * ph_texture = eir_gfx_create_texture(gfx_env, ph_atlas_image);
   eir_gfx_sprite_ref_t * ph_sprite_ref = eir_gfx_create_sprite_ref(gfx_env, ph_texture, 0, 0, 64, 64);

   // INIT WORLD ENTITIES

   eir_gme_set_world_capacity(gme_env, 1);
   eir_gme_world_t * world = eir_gme_create_world(gme_env, 10);
   eir_gme_entity_t entity = eir_gme_create_world_entity(world);

   eir_gme_position_component_t * position_cpt = eir_gme_set_entity_position(world, entity, 400, 300);
   eir_gme_set_entity_size(world, entity, 32, 32);
   eir_gme_set_entity_sprite_ref(world, entity, ph_sprite_ref);
   eir_gme_set_entity_color(world, entity, 0.0f, 1.0f, 0.0f, 0.5f);
   eir_gme_motion_param_component_t * motion_cpt = eir_gme_set_entity_acceleration(world, entity, 0.0f, 0.0f, PLAYER_SPEED, PLAYER_FRICTION);
   eir_gme_set_entity_aabb(world, entity, 0.0f, 0.0f, 32.0f, 32.0f);
   eir_gme_set_entity_camera(world, entity, 2.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
   eir_gme_set_entity_physic(world, entity, 1.0f);
	eir_gme_direction_component_t * direction_cpt = eir_gme_set_entity_direction(world, entity, EIR_GME_DIRECTION_BOTTOM);
	eir_gme_based_melee_attack_component_t * based_melee_att_cpt = eir_gme_set_entity_based_melee_attack(world, entity, 10, 0, 0, 64, 64, false);
   
   eir_gme_entity_t entity2 = eir_gme_create_world_entity(world);

   eir_gme_set_entity_position(world, entity2, 100, 100);
   eir_gme_set_entity_size(world, entity2, 64, 64);
   eir_gme_set_entity_sprite_ref(world, entity2, ph_sprite_ref);
   eir_gme_set_entity_color(world, entity2, 0.0f, 1.0f, 0.0f, 0.5f);
   eir_gme_set_entity_aabb(world, entity2, 0.0f, 0.0f, 128.0f, 128.0f);
   eir_gme_set_entity_physic(world, entity2, 1.0f);
   
   eir_gme_entity_t ground = eir_gme_create_world_entity(world);

   eir_gme_set_entity_position(world, ground, 0, 500);
   eir_gme_set_entity_size(world, ground, 1800, 32);
   eir_gme_set_entity_sprite_ref(world, ground, ph_sprite_ref);
   eir_gme_set_entity_color(world, ground, 1.0f, 1.0f, 1.0f, 0.5f);
   eir_gme_set_entity_aabb(world, ground, 0.0f, 0.0f, 1800.0f, 32.0f);
   eir_gme_set_entity_physic(world, ground, 1.0f);

	eir_gme_entity_t trigger = eir_gme_create_world_entity(world);
	
	eir_gme_set_entity_position(world, trigger, 600, 200);
	eir_gme_set_entity_size(world, trigger, 16, 16);
   eir_gme_set_entity_sprite_ref(world, trigger, ph_sprite_ref);
   eir_gme_set_entity_color(world, trigger, 1.0f, 1.0f, 1.0f, 0.5f);
   eir_gme_set_entity_aabb(world, trigger, 0.0f, 0.0f, 16.0f, 16.0f);

   eir_gme_set_active_world(gme_env, world);
   eir_gme_set_active_camera(world, entity);
   
   // INIT PLAYER USER DATA

   game.player.env = &env;
   game.player.owner = world;
   game.player.entity = entity;
   game.player.keyboard_buffer = eir_get_input_controller_buffer(gme_env, 0);
   game.player.pad_buffer = eir_get_input_controller_buffer(gme_env, 1);
	game.player.direction_component = direction_cpt;
	game.player.motion_param_component = motion_cpt;
	game.player.based_melee_attack_component = based_melee_att_cpt;

   // INIT STATE MACHINE

   eir_fsm_set_state_machine_capacity(fsm_env, 1);

   eir_fsm_state_machine_t * fsm = eir_fsm_create_state_machine(fsm_env, 10, &game.player);
   eir_fsm_state_t * idle_state = eir_fsm_create_state(fsm);
   eir_fsm_state_t * move_state = eir_fsm_create_state(fsm);
   eir_fsm_state_t * end_state = eir_fsm_create_state(fsm);
	eir_fsm_state_t * prepare_based_melee_attack_state = eir_fsm_create_state(fsm);
	eir_fsm_state_t * release_based_melee_attack_state = eir_fsm_create_state(fsm);

   eir_fsm_set_begin_state(fsm, idle_state);
   eir_fsm_set_end_state(fsm, end_state);
   
   eir_fsm_set_state_validate_func(idle_state, validate_idle_state);
   eir_fsm_set_state_validate_func(move_state, validate_move_state);
	eir_fsm_set_state_validate_func(prepare_based_melee_attack_state, validate_prepare_based_melee_attack_state);
	eir_fsm_set_state_validate_func(release_based_melee_attack_state, validate_release_based_melee_attack_state);

   eir_fsm_set_state_update_func(idle_state, update_idle_state);
   eir_fsm_set_state_update_func(move_state, update_move_state);
	eir_fsm_set_state_update_func(prepare_based_melee_attack_state, update_prepare_based_melee_attack_state);
	eir_fsm_set_state_update_func(release_based_melee_attack_state, update_release_based_melee_attack_state);

   eir_fsm_add_state_transition(idle_state, move_state);
   eir_fsm_add_state_transition(move_state, idle_state);
	eir_fsm_add_state_transition(idle_state, prepare_based_melee_attack_state);
	eir_fsm_add_state_transition(move_state, prepare_based_melee_attack_state);
	eir_fsm_add_state_transition(prepare_based_melee_attack_state, release_based_melee_attack_state);
	eir_fsm_add_state_transition(release_based_melee_attack_state, idle_state);
	eir_fsm_add_state_transition(release_based_melee_attack_state, move_state);

   game.player.fsm = fsm;

   // RUN EIR ENGINE

   eir_run(&game);

   // RELEASE ENV

   eir_release_env(&env);
   eir_display_mem_leaks();

   return 0;
}
