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
   eir_gfx_set_rect_capacity(gfx_env, 2);
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
   eir_fsm_run_state_machine(fsm_env);
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
