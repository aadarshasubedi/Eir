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

#include "kernel/eir_ker_env.h"

#include "fsm/eir_fsm_func.h"

#include "physics/eir_motion_func.h"

static void eir_init_all_api(eir_ker_env_t * env)
{
   eir_sys_win_api_init();
   eir_sys_win_api_create_window(&env->gfx_env);
   eir_gfx_api_init();
   eir_snd_api_init();
   eir_gfx_api_load_sprite_shaders(&env->gfx_env);
   eir_gfx_api_load_text_shaders(&env->gfx_env);
   eir_gfx_api_load_default_shaders(&env->gfx_env);
}

static void eir_stop(eir_ker_env_t * env)
{
   eir_gfx_api_unload_default_shaders(&env->gfx_env);
   eir_gfx_api_unload_text_shaders(&env->gfx_env);
   eir_gfx_api_unload_sprite_shaders(&env->gfx_env);
   eir_snd_api_release();
   eir_gfx_api_release();
   //eir_sys_close_joystick(&env->sys_env.joystick.handle); // TODO: close anywhere else
   eir_sys_win_api_destroy_window(&env->gfx_env);
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

/*
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
*/

static void eir_check_event_callback(eir_ker_env_t * env)
{
   if (env && !env->event_callback)
   {
      env->event_callback = eir_sys_default_event_callback;
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

eir_env_t * eir_create_env()
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
      env->event_callback = 0;
      eir_gfx_init_env(&env->gfx_env);
      eir_snd_init_env(&env->snd_env);
      eir_fsm_init_env(&env->fsm_env);
      eir_sys_init_env(&env->sys_env);
      eir_gme_init_env(&env->gme_env);
      eir_init_all_api(env);
   }
   return private_env;
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

   eir_gfx_set_text_capacity(gfx_env, 2);
   //eir_gfx_set_line_capacity(gfx_env, 10);
   //eir_gfx_set_quad_capacity(gfx_env, 10);
   //eir_snd_set_sound_capacity(snd_env, 2);


   //eir_init_player_state(&gme_env->player_1_state);

#ifdef EIR_DEBUG
   eir_mth_vec2_t position;
   eir_gfx_color_t color;
   eir_handle_t frame_rate_text_handle;

   position.x = -18.0f;
   position.y = 9.0f;
   color.r = 0.0f;
   color.g = 1.0f;
   color.b = 0.0f;
   color.a = 0.7f;
   frame_rate_text_handle = eir_gfx_add_text(gfx_env, "DEBUG TEST TEXT", &position, 1.0f, &color);
#endif

/*
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
*/

   // TODO: put anywhere else
   //if (eir_sys_get_joystick_count() > 0)
   //{
   //   sys_env->joystick.handle = eir_sys_get_joystick(0);
   //   gme_env->player_1_state.pad_index = 0;
   //}
   // -------------------------

   // TODO: to remove 
   //sound_handle = eir_snd_load_sound_file(snd_env, "../resources/sounds/medium.wav");
   // -------------------------

   float time_per_frame = 1.0f / 60.0f;
   float time_since_last_update = 0.0f;

   eir_sys_start_timer(&sys_env->timer);
   eir_fsm_run_state_machine(fsm_env);
   eir_gfx_generate_all_batches(gfx_env, gme_env->curr_world);
   for (;;)
   {
      eir_sys_update_timer(&sys_env->timer);
      time_since_last_update += sys_env->timer.elapsed_time;
      while (time_since_last_update > time_per_frame)
      {
	 time_since_last_update -= time_per_frame;
	 if (!eir_sys_win_api_poll_all_events(all_env->event_callback, env))
	 {
	    return;
	 }
	 // UPDATE ALL SYSTEMS HERE EXCEPT RENDERING AND TIMER
	 eir_fsm_update_state_machine(fsm_env);
	 eir_phy_proceed_motion_entity_update(gme_env->curr_world, time_per_frame);

	 // TODO: update only real modified batch
	 eir_gfx_force_update_all_batches(gfx_env);
      }

#ifdef EIR_DEBUG
      eir_render_frame_rate(gfx_env, sys_env->timer.elapsed_time, frame_rate_text_handle);
#endif
      eir_gfx_api_set_clear_color();
      eir_gfx_api_clear_buffer();
      eir_gfx_render_all_batches(gfx_env);
      eir_sys_win_api_swap_buffer(gfx_env);

      // TODO: remove when plauer state system fully implemented
      //gfx_env->sprite_batches.data[0].sprites.data[0].position.x = gme_env->player_1_state.position.x;
      //gfx_env->sprite_batches.data[0].sprites.data[0].position.y = gme_env->player_1_state.position.y;
      //gfx_env->sprite_batches.data[0].modified = true;
      // -------------------------
      // TODO: remove when event sound system up
      //if (sys_env->joystick.x_axis_value != 0)
      //{
      // eir_snd_play_sound(snd_env, sound_handle);
      //}
      // -------------------------
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
