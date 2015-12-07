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
#include "system/eir_event_callback.h"

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

#include "kernel/eir_env.h"

#include "fsm/eir_fsm_func.h"

static void eir_start(eir_gfx_env_t * gfx_env, eir_sys_env_t * sys_env)
{
   eir_sys_win_api_init();
   eir_sys_win_api_create_window(gfx_env);
   eir_gfx_api_init();
   eir_snd_api_init();
   eir_sys_init(sys_env);
   eir_gfx_api_load_sprite_shaders(gfx_env);
   eir_gfx_api_load_text_shaders(gfx_env);
   eir_gfx_api_load_default_shaders(gfx_env);
}

static void eir_stop(eir_gfx_env_t * gfx_env, eir_sys_env_t * sys_env, eir_snd_env_t * snd_env)
{
   eir_gfx_api_unload_default_shaders(gfx_env);
   eir_gfx_api_unload_text_shaders(gfx_env);
   eir_gfx_api_unload_sprite_shaders(gfx_env);
   eir_snd_release_all_sounds(snd_env);
   eir_snd_api_release();
   eir_sys_close_joystick(sys_env->joystick.handle);
   eir_gfx_release_all_batches(gfx_env);
   eir_sys_win_api_destroy_window(gfx_env);
   EIR_KER_RELEASE_ARRAY(gfx_env->sprite_batches);
   EIR_KER_RELEASE_ARRAY(gfx_env->text_batches);
   EIR_KER_RELEASE_ARRAY(snd_env->sounds);
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

static void eir_init_player_state(eir_gme_player_state_t * player_state)
{
   if (!player_state)
   {
      return;
   }
   player_state->position.x = 0.0f;
   player_state->position.y = 0.0f;
   player_state->motion_param.velocity.x = 0.0f;
   player_state->motion_param.velocity.y = 0.0f;
   player_state->motion_param.max_velocity.x = 5.0f;
   player_state->motion_param.max_velocity.y = 5.0f;
   player_state->motion_param.acceleration.x = 0.0f;
   player_state->motion_param.acceleration.y = 0.0f;
}

static void eir_check_event_callback(eir_ker_env_t * env)
{
   if (env && !env->event_callback)
   {
      env->event_callback = eir_sys_default_event_callback;
   }
}

void eir_set_allocate_func(eir_allocate_t allocate_func)
{
   eir_sys_allocate = allocate_func;
}

void eir_set_free_func(eir_free_t free_func)
{
   eir_sys_free = free_func;
}

eir_env_t * eir_create_env()
{
   eir_check_allocate_and_free_func();

   eir_env_t * env = eir_sys_allocate(sizeof(eir_env_t), 1);

   if (env)
   {
      env->private = eir_sys_allocate(sizeof(eir_ker_env_t), 1);
      ((eir_ker_env_t *)(env->private))->event_callback = 0;
   }
   return env;
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

   eir_check_event_callback(all_env);

   eir_gfx_env_t * gfx_env = &all_env->gfx_env;
   eir_sys_env_t * sys_env = &all_env->sys_env;
   eir_snd_env_t * snd_env = &all_env->snd_env;
   eir_gme_env_t * gme_env = &all_env->gme_env;
   eir_fsm_env_t * fsm_env = &all_env->fsm_env;

   if (!gfx_env || !sys_env || !snd_env || !gme_env)
   {
      return;
   }

   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;
   eir_gfx_color_t color;
   eir_gfx_sprite_batch_t * sprite_batch = 0;
   eir_handle_t batch_handle;
   eir_handle_t sprite_handle;
   eir_handle_t sound_handle;
   eir_handle_t text_handle;

   eir_gfx_set_batch_capacity(gfx_env, 2);
   eir_gfx_set_text_capacity(gfx_env, 2);
   eir_gfx_set_line_capacity(gfx_env, 10);
   eir_gfx_set_quad_capacity(gfx_env, 10);
   eir_snd_set_sound_capacity(snd_env, 2);

   batch_handle = eir_gfx_create_sprite_batch(gfx_env, 2);

   eir_init_player_state(&gme_env->player_1_state);

   position.x = gme_env->player_1_state.position.x;
   position.y = gme_env->player_1_state.position.y;
   size.x = 1.0f;
   size.y = 1.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   color.r = 1.0f;
   color.g = 0.0f;
   color.b = 0.0f;
   color.a = 1.0f;
   eir_gfx_add_sprite(gfx_env, &position, &size, &uv_offset, &uv_size, &color, batch_handle);

   position.x = 2.0f;
   position.y = 2.0f;
   size.x = 2.0f;
   size.y = 2.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 64.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   color.r = 1.0f;
   color.g = 1.0f;
   color.b = 1.0f;
   color.a = 1.0f;
   eir_gfx_add_sprite(gfx_env, &position, &size, &uv_offset, &uv_size, &color, batch_handle);

   position.x = -18.0f;
   position.y = 9.0f;
   color.r = 0.0f;
   color.g = 0.0f;
   color.b = 1.0f;
   color.a = 1.0f;
   text_handle = eir_gfx_add_text(gfx_env, "DEBUG TEST TEXT", &position, 1.0f, &color);

   eir_mth_vec2_t line_a;
   eir_mth_vec2_t line_b;
   eir_gfx_color_t line_a_color;
   eir_gfx_color_t line_b_color;

   line_a.x = 0.9f;
   line_a.y = 0.0f;
   line_b.x = -0.9f;
   line_a.y = 0.9f;
   line_a_color.r = 1.0f;
   line_a_color.g = 0.0f;
   line_a_color.b = 0.0f;
   line_a_color.a = 1.0f;
   line_b_color.r = 0.0f;
   line_b_color.g = 0.0f;
   line_b_color.b = 1.0f;
   line_b_color.a = 0.5f;
   eir_gfx_add_line(gfx_env, &line_a, &line_b, &line_a_color, &line_b_color);

   line_a.x = 0.2f;
   line_a.y = 0.2f;
   line_b.x = -0.5f;
   line_b.y = 0.2f;
   line_a_color.r = 0.5f;
   line_a_color.g = 0.5f;
   line_a_color.b = 0.0f;
   line_a_color.a = 1.0f;
   line_b_color.r = 0.0f;
   line_b_color.g = 0.5f;
   line_b_color.b = 1.0f;
   line_b_color.a = 1.0f;
   eir_gfx_add_line(gfx_env, &line_a, &line_b, &line_a_color, &line_b_color);

   line_a.x = 0.5f;
   line_a.y = -0.5f;
   line_b.x = 0.5f;
   line_b.y = 0.5f;
   line_a_color.r = 1.0f;
   line_a_color.g = 0.5f;
   line_a_color.b = 1.0f;
   line_a_color.a = 1.0f;
   line_b_color.r = 0.0f;
   line_b_color.g = 1.0f;
   line_b_color.b = 1.0f;
   line_b_color.a = 1.0f;
   eir_gfx_add_line(gfx_env, &line_a, &line_b, &line_a_color, &line_b_color);

   position.x = -0.2f;
   position.y = -0.5f;
   size.x = 0.5f;
   size.y = 0.5f;
   color.r = 0.8f;
   color.g = 0.8f;
   color.b = 0.8f;
   color.a = 1.0f;
   eir_gfx_add_quad(gfx_env, &position, &size, &color);

   eir_start(gfx_env, sys_env);

   // TODO: put anywhere else
   if (eir_sys_get_joystick_count() > 0)
   {
      sys_env->joystick.handle = eir_sys_get_joystick(0);
      gme_env->player_1_state.pad_index = 0;
   }
   // -------------------------

   eir_snd_api_init();

   // TODO: to remove 
   sound_handle = eir_snd_load_sound_file(snd_env, "../resources/sounds/medium.wav");
   // -------------------------

   eir_sys_start_timer(&sys_env->timer);
   eir_fsm_run_state_machine(fsm_env);
   for (;;)
   {
      eir_sys_update_timer(&sys_env->timer);

      char c[32];
      sprintf(c, "frame rate: %1.3f", sys_env->timer.elapsed_time);
      eir_gfx_update_text(gfx_env, text_handle, c);

      if (!eir_sys_win_api_poll_all_events(all_env->event_callback, env))
      {
	 break;
      }

      eir_fsm_update_state_machine(fsm_env);

      eir_gfx_api_set_clear_color();
      eir_gfx_api_clear_buffer();
      
      // TODO: remove when plauer state system fully implemented
      gfx_env->sprite_batches.data[0].sprites.data[0].position.x = gme_env->player_1_state.position.x;
      gfx_env->sprite_batches.data[0].sprites.data[0].position.y = gme_env->player_1_state.position.y;
      gfx_env->sprite_batches.data[0].modified = true;
      // -------------------------

      // TODO: remove when event sound system up
      //if (sys_env->joystick.x_axis_value != 0)
      //{
      // eir_snd_play_sound(snd_env, sound_handle);
      //}
      // -------------------------

      eir_gfx_render_all_batches(gfx_env);
      eir_sys_win_api_swap_buffer(gfx_env);
   }
   EIR_KER_LOG_MESSAGE("stop eir");
   eir_fsm_release_env(fsm_env);
   eir_stop(gfx_env, sys_env, snd_env);
   EIR_SYS_LOG_ALLOCATED_ELEM;
}

void eir_destroy_env(eir_env_t * env)
{
   if (env)
   {
      if (env->private)
      {
	 eir_sys_free(env->private);
	 env->private = 0;
      }
      eir_sys_free(env);
      env = 0;
   }
}
