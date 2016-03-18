#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "maths/eir_mth_vector.h"
#include "system/eir_sys_env.h"
#include "system/eir_sys_timer_func.h"
#include "system/eir_sys_file_system.h"
#include "system/eir_sys_memory.h"
#include "system/eir_sys_joystick_func.h"
#include "system/eir_sys_win_api_func.h"
#include "graphics/eir_gfx_sprite.h"
#include "graphics/eir_gfx_types.h"
#include "graphics/eir_gfx_defines.h"
#include "graphics/eir_gfx_sprite_batch.h"
#include "graphics/eir_gfx_env.h"
#include "graphics/eir_gfx_api_func.h"
#include "graphics/eir_gfx_func.h"
#include "sound/eir_snd_api_func.h"
#include "sound/eir_snd_env.h"
#include "game/eir_gme_env.h"
#include "game/eir_gme_func.h"
#include "game/eir_gme_system.h"
#include "game/eir_gme_ground_template.h"
#include "kernel/eir_ker_env.h"
#include "fsm/eir_fsm_func.h"
#include "physics/eir_phy_motion_func.h"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 728; 

const float PLAYER_FRICTION = 10.0f;
const float PLAYER_SPEED = 1600.0f;

/*
typedef struct
{
   eir_ker_env_t * env;
   eir_gme_world_t * owner;
   eir_gme_entity_t entity;
   eir_gme_input_controller_buffer_t * keyboard_buffer;
   eir_gme_input_controller_buffer_t * pad_buffer;
} player_t;

typedef struct
{
   player_t player;
} game_t;
*/

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

static void eir_render_frame_rate(
   eir_gfx_text_t * gfx_text,
   const eir_mth_vec2_t * position,
   float font_size,
   const eir_gfx_color_t * color,
   float elapsed_time
   )
{
   char c[32];
   sprintf(c, "frame rate: %1.3f", elapsed_time);
   eir_gfx_modify_text(gfx_text, c, position, font_size, color);
}

static void eir_render_player_direction(
   eir_gfx_text_t * gfx_text,
   const eir_mth_vec2_t * position,
   float font_size,
   const eir_gfx_color_t * color,
   eir_gme_direction_t direction
   )
{
   if (direction == EIR_GME_DIRECTION_UP)
   {
      eir_gfx_modify_text(gfx_text, "direction: up", position, font_size, color);
   }
   else if (direction == EIR_GME_DIRECTION_UP_RIGHT)
   {
      eir_gfx_modify_text(gfx_text, "direction: up/right", position, font_size, color);
   }
   else if (direction == EIR_GME_DIRECTION_RIGHT)
   {
      eir_gfx_modify_text(gfx_text, "direction: right", position, font_size, color);
   }
   else if (direction == EIR_GME_DIRECTION_BOTTOM_RIGHT)
   {
      eir_gfx_modify_text(gfx_text, "direction: bottom/right", position, font_size, color);
   }
   else if (direction == EIR_GME_DIRECTION_BOTTOM)
   {
      eir_gfx_modify_text(gfx_text, "direction: bottom", position, font_size, color);
   }
   else if (direction == EIR_GME_DIRECTION_BOTTOM_LEFT)
   {
      eir_gfx_modify_text(gfx_text, "direction: bottom/left", position, font_size, color);
   }
   else if (direction == EIR_GME_DIRECTION_LEFT)
   {
      eir_gfx_modify_text(gfx_text, "direction: left", position, font_size, color);
   }
   else if (direction == EIR_GME_DIRECTION_UP_LEFT)
   {
      eir_gfx_modify_text(gfx_text, "direction: up/left", position, font_size, color);
   }
   else 
   {
      eir_gfx_modify_text(gfx_text, "direction: unknown", position, font_size, color);
   }
}

static void eir_render_player_velocity(
   eir_gfx_text_t * gfx_text,
   const eir_mth_vec2_t * position,
   float font_size,
   const eir_gfx_color_t * color,
   float x,
   float y
   )
{
   char c [32];
   sprintf(c, "velocity: (%1.3f; %1.3f)", x, y);
   eir_gfx_modify_text(gfx_text, c, position, font_size, color);
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

static void eir_run(eir_ker_env_t * env)
{
   //player_t * player = &game->player;
   //eir_ker_env_t * env = player->env;
   eir_gfx_env_t * gfx_env = &env->gfx_env;
   eir_sys_env_t * sys_env = &env->sys_env;
   eir_snd_env_t * snd_env = &env->snd_env;
   eir_gme_env_t * gme_env = &env->gme_env;
   eir_fsm_env_t * fsm_env = &env->fsm_env;

   if (!gfx_env || !sys_env || !snd_env || !gme_env)
   {
      return;
   }

#ifdef EIR_DEBUG

   eir_gfx_group_t * debug_info_group = eir_gfx_create_group(gfx_env, 0, 3, 0);

   eir_gfx_text_t * frame_text = 0;
   eir_mth_vec2_t frame_text_pos;
   float frame_text_font_size = 32.0f;
   eir_gfx_color_t frame_text_color;

   frame_text_pos.x = 0.0f;
   frame_text_pos.y = 0.0f;
   frame_text_color.r = 0.0f;
   frame_text_color.g = 1.0f;
   frame_text_color.b = 0.0f;
   frame_text_color.a = 0.7f;
   frame_text = eir_gfx_add_text_to_group(
      debug_info_group,
         "DEBUG",
         &frame_text_pos,
         frame_text_font_size,
         &frame_text_color,
         true,
      true,
      true
      );

   eir_gfx_text_t * player_info_text = 0;
   eir_mth_vec2_t player_info_text_pos;
   float player_info_text_font_size = 16.0f;
   eir_gfx_color_t player_info_text_color;

   player_info_text_pos.x = 0.0f;
   player_info_text_pos.y = 40.0f;
   player_info_text_color.r = 1.0f;
   player_info_text_color.g = 0.0f;
   player_info_text_color.b = 0.0f;
   player_info_text_color.a = 0.7f;
   player_info_text = eir_gfx_add_text_to_group(
      debug_info_group,
         "DEBUG",
         &player_info_text_pos,
         player_info_text_font_size,
         &player_info_text_color,
         true,
      true,
      true
      );

   eir_gfx_text_t * player_velocity_text = 0;
   eir_mth_vec2_t player_velocity_text_pos;
   float player_velocity_text_font_size = 16.0f;
   eir_gfx_color_t player_velocity_text_color;

   player_velocity_text_pos.x = 0.0f;
   player_velocity_text_pos.y = 60.0f;
   player_velocity_text_color.r = 1.0f;
   player_velocity_text_color.g = 0.0f;
   player_velocity_text_color.b = 0.0f;
   player_velocity_text_color.a = 0.7f;
   player_velocity_text = eir_gfx_add_text_to_group(
      debug_info_group,
         "DEBUG",
         &player_velocity_text_pos,
         player_velocity_text_font_size,
         &player_velocity_text_color,
         true,
      true,
      true
      );
#endif

   double time_per_frame = 1.0f / 60.0f;
   double time_since_last_update = 0.0f;

   eir_sys_start_timer(&sys_env->timer);
   eir_gme_start_all_components_systems(gme_env->curr_world);
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

         eir_gme_update_all_components_systems(gme_env->curr_world, time_per_frame);

         if (gme_env->curr_world)
         {
            eir_gfx_update_camera_view(gfx_env, &gme_env->curr_world->camera.position);
         }
      }

#ifdef EIR_DEBUG
      eir_render_frame_rate(
         frame_text,
         &frame_text_pos,
         frame_text_font_size,
         &frame_text_color,
         sys_env->timer.elapsed_time
         );
      /*
      eir_render_player_direction(
         player_info_text,
         &player_info_text_pos,
         player_info_text_font_size,
         &player_info_text_color,
         player->direction_component->value
         );
      eir_render_player_velocity(
         player_velocity_text,
         &player_velocity_text_pos,
         player_velocity_text_font_size,
         &player_velocity_text_color,
         player->motion_param_component->data.velocity.x,
         player->motion_param_component->data.velocity.y
         );
      */
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
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;
      eir_gme_world_t * world = entity_proxy->world;
      eir_gme_entity_t entity = entity_proxy->entity;
      eir_gme_input_controller_buffer_t * pad_buffer = world->pads.data[entity].input_buffer;
      eir_gme_input_controller_buffer_t * keyboard_buffer = world->keyboards.data[entity].input_buffer;

      if (!pad_buffer->controllers[1].is_analog)
      {
         if (
            keyboard_buffer
            && pad_buffer
            && !pad_buffer->controllers[1].is_connected
            && !keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
            && !keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
            && !keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
            && !keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
            && !keyboard_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
            )
         {
            result = true;
         }
         else if (
            pad_buffer
            && pad_buffer->controllers[1].is_connected
            && !pad_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
            && !pad_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
            && !pad_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
            && !pad_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
            && !pad_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
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
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;
      eir_gme_world_t * world = entity_proxy->world;
      eir_gme_entity_t entity = entity_proxy->entity;
      eir_gme_input_controller_buffer_t * pad_buffer = world->pads.data[entity].input_buffer;
      eir_gme_input_controller_buffer_t * keyboard_buffer = world->keyboards.data[entity].input_buffer;

      if (
         keyboard_buffer
         && pad_buffer
         && !pad_buffer->controllers[1].is_connected
         && (
            keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
            || keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
            || keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
            || keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
            )
         )
      {
         result = true;
      }
      else if (
         pad_buffer
         && pad_buffer->controllers[1].is_connected
         && !pad_buffer->controllers[1].is_analog
         && (
            pad_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
            || pad_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
            || pad_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
            || pad_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
            )
         )
      {
         result = true;
      }
      else if (
         pad_buffer
         && pad_buffer->controllers[1].is_connected
         && pad_buffer->controllers[1].is_analog
         && (
            pad_buffer->controllers[1].left_stick_value_x
            || pad_buffer->controllers[1].left_stick_value_y
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
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;
      eir_gme_world_t * world = entity_proxy->world;
      eir_gme_entity_t entity = entity_proxy->entity;
      eir_gme_input_controller_buffer_t * pad_buffer = world->pads.data[entity].input_buffer;
      eir_gme_input_controller_buffer_t * keyboard_buffer = world->keyboards.data[entity].input_buffer;

      if (
         keyboard_buffer
         && pad_buffer
         && !pad_buffer->controllers[1].is_connected
         && keyboard_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
         )
      {
         result = true;
      }
      else if (
         pad_buffer
         && pad_buffer->controllers[1].is_connected
         && pad_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
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
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;
      eir_gme_world_t * world = entity_proxy->world;
      eir_gme_entity_t entity = entity_proxy->entity;
      eir_gme_input_controller_buffer_t * pad_buffer = world->pads.data[entity].input_buffer;
      eir_gme_input_controller_buffer_t * keyboard_buffer = world->keyboards.data[entity].input_buffer;

      if (
         keyboard_buffer
         && pad_buffer
         && !pad_buffer->controllers[1].is_connected
         && !keyboard_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
         )
      {
         result = true;
      }
      else if (
         pad_buffer
         && pad_buffer->controllers[1].is_connected
         && !pad_buffer->controllers[1].buttons[EIR_GME_ACTION_1_BUTTON_INDEX].pressed
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
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;

      eir_gme_set_entity_acceleration(
         entity_proxy->world,
         entity_proxy->entity,
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
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;
      eir_gme_world_t * world = entity_proxy->world;
      eir_gme_entity_t entity = entity_proxy->entity;
      eir_gme_input_controller_buffer_t * pad_buffer = world->pads.data[entity].input_buffer;
      eir_gme_input_controller_buffer_t * keyboard_buffer = world->keyboards.data[entity].input_buffer;

      float x_velocity = 0.0f;
      float y_velocity = 0.0f;

      if (pad_buffer->controllers[1].is_analog)
      {
         x_velocity = pad_buffer->controllers[1].left_stick_value_x;
         y_velocity = pad_buffer->controllers[1].left_stick_value_y;
      }
      else
      {
         if (
            keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
            || pad_buffer->controllers[1].buttons[EIR_GME_MOVE_RIGHT_BUTTON_INDEX].pressed
            )
         {
            x_velocity = 1.0f;
         }
         if (
            keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
            || pad_buffer->controllers[1].buttons[EIR_GME_MOVE_LEFT_BUTTON_INDEX].pressed
            )
         {
            x_velocity = -1.0f;
         }
         if (
            keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
            || pad_buffer->controllers[1].buttons[EIR_GME_MOVE_DOWN_BUTTON_INDEX].pressed
            )
         {
            y_velocity = 1.0f;
         }
         if (
            keyboard_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
            || pad_buffer->controllers[1].buttons[EIR_GME_MOVE_UP_BUTTON_INDEX].pressed
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
         world,
         entity,
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
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;

      eir_gme_set_entity_acceleration(
         entity_proxy->world,
         entity_proxy->entity,
         0.0f,
         0.0f, 
         PLAYER_SPEED,
         PLAYER_FRICTION
         );
   }
}

static void update_release_based_melee_attack_state(void * user_data)
{
   if (user_data)
   {
      eir_gme_entity_proxy_t * entity_proxy = (eir_gme_entity_proxy_t *)user_data;
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

   // INIT PLAYER USER DATA

   //game_t game;

   //game.player.env = &env;
   //game.player.keyboard_buffer = eir_get_input_controller_buffer(gme_env, 0);
   //game.player.pad_buffer = eir_get_input_controller_buffer(gme_env, 1);

   eir_gme_entity_proxy_t player_entity_proxy;

   // INIT STATE MACHINE

   eir_fsm_set_state_machine_capacity(fsm_env, 1);

   eir_fsm_state_machine_t * fsm = eir_fsm_create_state_machine(fsm_env, 10, &player_entity_proxy);
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

      // INIT GFX ITEMS CAPACITY

   eir_gfx_set_image_capacity(gfx_env, 10);
   eir_gfx_set_texture_capacity(gfx_env, 10);
   eir_gfx_set_group_capacity(gfx_env, 10);

      // LOAD SPRITE

   eir_gfx_image_t * ph_atlas_image = eir_gfx_load_image(gfx_env, PLACE_HOLDER_IMAGE_PATH, false);
   eir_gfx_texture_t * ph_texture = eir_gfx_create_texture(gfx_env, ph_atlas_image);

   eir_gfx_image_t * ph_tile_image = eir_gfx_load_image(
      gfx_env,
      PLACE_HOLDER_TILE_PATH,
      false 
      );
   eir_gfx_texture_t * ph_tile_texture = eir_gfx_create_texture(
      gfx_env,
      ph_tile_image
      );

   // COMMON STRUCT

   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_color_t color;

   // TEMP GROUND GRID

   eir_gme_ground_template_t ground_template;

   ground_template.col_count = 40;
   ground_template.row_count = 40;
   ground_template.position.x = 0.0f;
   ground_template.position.y = 0.0f;

   eir_gfx_group_t * ground_group = eir_gfx_create_group(gfx_env, 1, 0, 0);
   eir_gfx_sprite_batch_t * ground_batch = eir_gfx_add_sprite_batch_to_group(
      ground_group,
      ph_tile_texture,
      ground_template.col_count * ground_template.row_count,
      true,
      false,
      true
      );

   position.x = 0.0f;
   position.y = 0.0f;
   size.x = 64.0f;
   size.y = 32.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 32.0f;
   color.r = 1.0f;
   color.g = 1.0f;
   color.b = 1.0f;
   color.a = 1.0f;

   for (int c = 0; c < ground_template.col_count; ++c)
   {
      for (int r = 0; r < ground_template.row_count; ++r)
      {
         position.x = ground_template.position.x + c * size.x;
         position.y = ground_template.position.y + r * size.y;
         eir_gfx_add_sprite_to_batch(
            ground_batch,
            &position,
            &size,
            &uv_offset,
            &uv_size,
            &color,
            true
            );
      }
   }

   // CREATE SPRITE

   eir_gfx_group_t * sprites_group = eir_gfx_create_group(gfx_env, 10, 0, 0);
   eir_gfx_sprite_batch_t * sprites_batch = eir_gfx_add_sprite_batch_to_group(
      sprites_group,
      ph_texture,
      3,
      true,
      false,
      true
      );

   position.x = 0.0f;
   position.y = 0.0f;
   size.x = 64.0f;
   size.y = 64.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   color.r = 1.0f;
   color.g = 0.0f;
   color.b = 0.0f;
   color.a = 0.2f;
   eir_gfx_sprite_proxy_t * obj_sprite = eir_gfx_add_sprite_to_batch(
      sprites_batch,
      &position,
      &size,
      &uv_offset,
      &uv_size,
      &color,
      true
      );

   position.x = 0.0f;
   position.y = 0.0f;
   size.x = 64.0f;
   size.y = 64.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   color.r = 1.0f;
   color.g = 0.0f;
   color.b = 0.0f;
   color.a = 0.2f;
   eir_gfx_sprite_proxy_t * player_sprite = eir_gfx_add_sprite_to_batch(
      sprites_batch,
      &position,
      &size,
      &uv_offset,
      &uv_size,
      &color,
      true
      );

   position.x = 0.0f;
   position.y = 0.0f;
   size.x = 64.0f;
   size.y = 64.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   color.r = 1.0f;
   color.g = 0.0f;
   color.b = 0.0f;
   color.a = 0.2f;
   eir_gfx_sprite_proxy_t * wall_sprite = eir_gfx_add_sprite_to_batch(
      sprites_batch,
      &position,
      &size,
      &uv_offset,
      &uv_size,
      &color,
      true
      );

   // CREATE RECT

   eir_gfx_group_t * rect_group = eir_gfx_create_group(gfx_env, 0, 0, 10);
   eir_gfx_rect_batch_t * rect_batch = eir_gfx_add_rect_batch_to_group(
      rect_group,
      3,
      true,
      false,
      true
      );

   position.x = 0.0f;
   position.y = 64.0f;
   size.x = 64.0f;
   size.y = 64.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   color.r = 1.0f;
   color.g = 1.0f;
   color.b = 0.0f;
   color.a = 0.2f;
   eir_gfx_rect_proxy_t  * entity_aabb_rect_proxy = eir_gfx_add_rect_to_batch(
         rect_batch,
         &position,
      &size,
         &color,
         true
         );

   position.x = 0.0f;
   position.y = 128.0f;
   size.x = 64.0f;
   size.y = 64.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   color.r = 1.0f;
   color.g = 1.0f;
   color.b = 0.0f;
   color.a = 0.2f;
   eir_gfx_rect_proxy_t  * wall_aabb_rect_proxy = eir_gfx_add_rect_to_batch(
         rect_batch,
         &position,
      &size,
         &color,
         true
         );

      // INIT WORLD ENTITIES

   eir_gme_set_world_capacity(gme_env, 1);
   eir_gme_world_t * world = eir_gme_create_world(gme_env, 10);
   eir_gme_entity_t entity = eir_gme_create_world_entity(world);

   eir_gme_set_entity_position(world, entity, 0.0f, 0.0f);
   eir_gme_set_entity_size(world, entity, 64, 64);
   eir_gme_set_entity_sprite(world, entity, player_sprite);
   eir_gme_set_entity_color(world, entity, 0.0f, 1.0f, 0.0f, 0.5f);
   eir_gme_set_entity_acceleration(world, entity, 0.0f, 0.0f, PLAYER_SPEED, PLAYER_FRICTION);
   eir_gme_set_entity_aabb(world, entity, 0.0f, 0.0f, 64.0f, 64.0f);
   eir_gme_set_entity_aabb_primitive(world, entity, entity_aabb_rect_proxy);
   eir_gme_set_entity_physic(world, entity, 0.5f);
   eir_gme_set_entity_direction(world, entity, EIR_GME_DIRECTION_BOTTOM);
   //eir_gme_set_entity_based_melee_attack(world, entity, 10, 0, 0, 64, 64, false);
   eir_gme_set_entity_fsm(world, entity, fsm);
   eir_gme_set_entity_keyboard_controller(
      world,
      entity,
      eir_get_input_controller_buffer(gme_env, 0)
      );
   eir_gme_set_entity_pad_controller(
      world,
      entity,
      eir_get_input_controller_buffer(gme_env, 1)
      );

   player_entity_proxy.entity = entity;
   player_entity_proxy.world = world;

   //game.player.owner = world;
   //game.player.entity = entity;

   eir_gme_entity_t entity2 = eir_gme_create_world_entity(world);

   eir_gme_set_entity_position(world, entity2, 200.0f, 0.0f);
   eir_gme_set_entity_size(world, entity2, 64, 64);
   eir_gme_set_entity_sprite(world, entity2, obj_sprite);
   eir_gme_set_entity_color(world, entity2, 1.0f, 0.0f, 0.0f, 0.5f);
   eir_gme_set_entity_aabb(world, entity2, 0.0f, 0.0f, 64.0f, 64.0f);
   eir_gme_set_entity_physic(world, entity2, 1.0f);

   eir_gme_entity_t wall = eir_gme_create_world_entity(world);

   eir_gme_set_entity_position(world, wall, 400, 300);
   eir_gme_set_entity_size(world, wall, 1800, 32);
   eir_gme_set_entity_sprite(world, wall, wall_sprite);
   eir_gme_set_entity_color(world, wall, 1.0f, 1.0f, 1.0f, 0.5f);
   eir_gme_set_entity_aabb(world, wall, 0.0f, 0.0f, 1800.0f, 32.0f);
   eir_gme_set_entity_aabb_primitive(world, wall, wall_aabb_rect_proxy);
   eir_gme_set_entity_physic(world, wall, 1.0f);

   eir_gme_set_active_world(gme_env, world);
   eir_gme_set_active_camera(world, entity, 4.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

   // RUN EIR ENGINE

   eir_run(&env);

   // RELEASE ENV

   eir_release_env(&env);
   eir_display_mem_leaks();

   return 0;
}
