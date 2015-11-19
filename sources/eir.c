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
#include "graphics/eir_sprite.h"
#include "graphics/eir_gfx_types.h"
#include "graphics/eir_gfx_defines.h"
#include "graphics/eir_sprite_batch.h"
#include "graphics/eir_gfx_env.h"
#include "graphics/eir_gfx_api_func.h"
#include "graphics/eir_gfx_func.h"
#include "window/eir_win_api_func.h"
#include "sound/eir_snd_api_func.h"
#include "sound/eir_snd_env.h"

static void eir_start(eir_gfx_env_t * gfx_env, eir_sys_env_t * sys_env)
{
   eir_win_api_init();
   eir_win_api_create_window(gfx_env);
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
   eir_win_api_destroy_window(gfx_env);
   EIR_KER_RELEASE_ARRAY(gfx_env->sprite_batches);
   EIR_KER_RELEASE_ARRAY(snd_env->sounds);
}

void eir_run()
{
   eir_sys_allocate = eir_sys_default_allocate;
   eir_sys_free = eir_sys_default_free;

   eir_gfx_env_t gfx_env;
   eir_sys_env_t sys_env;
   eir_snd_env_t snd_env;
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

   eir_gfx_set_batch_capacity(&gfx_env, 2);
   eir_snd_set_sound_capacity(&snd_env, 2);
   eir_gfx_set_text_capacity(&gfx_env, 2);
   eir_gfx_set_line_capacity(&gfx_env, 10);

   batch_handle = eir_gfx_create_empty_batch(&gfx_env, 2);

   position.x = -10.0f;
   position.y = 0.0f;
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
   eir_gfx_add_sprite_to_batch(&gfx_env, &position, &size, &uv_offset, &uv_size, &color, batch_handle);

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
   eir_gfx_add_sprite_to_batch(&gfx_env, &position, &size, &uv_offset, &uv_size, &color, batch_handle);

   position.x = -18.0f;
   position.y = 9.0f;
   color.r = 0.0f;
   color.g = 0.0f;
   color.b = 1.0f;
   color.a = 1.0f;
   text_handle = eir_gfx_add_text(&gfx_env, "DEBUG TEST TEXT", &position, 1.0f, &color);

   eir_mth_vec2_t line_a;
   eir_mth_vec2_t line_b;
   eir_gfx_color_t line_a_color;
   eir_gfx_color_t line_b_color;

   line_a.x = 1.0f;
   line_a.y = 0.0f;
   line_a.x = -1.0f;
   line_a.y = 1.0f;
   line_a_color.r = 1.0f;
   line_a_color.g = 0.0f;
   line_a_color.b = 0.0f;
   line_a_color.a = 1.0f;
   line_a_color.r = 0.0f;
   line_a_color.g = 0.0f;
   line_a_color.b = 1.0f;
   line_a_color.a = 0.5f;
   eir_gfx_add_line(&gfx_env, &line_a, &line_b, &line_a_color, &line_b_color);

   eir_start(&gfx_env, &sys_env);
   if (eir_sys_get_joystick_count() > 0)
   {
      sys_env.joystick.handle = eir_sys_get_joystick(0);
   }
   eir_snd_api_init();

   sound_handle = eir_snd_load_sound_file(&snd_env, "../resources/sounds/medium.wav");
   eir_sys_start_timer(&sys_env.timer);

   for (;;)
   {
      eir_sys_update_timer(&sys_env.timer);

      char c[32];
      sprintf(c, "frame rate: %1.3f", sys_env.timer.elapsed_time);
      eir_gfx_update_text(&gfx_env, text_handle, c);

      if (!eir_win_api_poll_all_events(&sys_env.joystick))
      {
	 break;
      }
      eir_gfx_api_set_clear_color();
      eir_gfx_api_clear_buffer();
      
      // TODO: to remove after test
      gfx_env.sprite_batches.data[0].sprites.data[0].position.x += sys_env.joystick.x_axis_value * sys_env.timer.elapsed_time;
      gfx_env.sprite_batches.data[0].sprites.data[0].color.a += sys_env.timer.elapsed_time;
      if (gfx_env.sprite_batches.data[0].sprites.data[0].color.a > 1.0f)
      {
	 gfx_env.sprite_batches.data[0].sprites.data[0].color.a = 0.0f;
      }
      gfx_env.sprite_batches.data[0].modified = true;
      if (sys_env.joystick.x_axis_value != 0)
      {
	 eir_snd_play_sound(&snd_env, sound_handle);
      }
      // -------------------------

      eir_gfx_render_all_batches(&gfx_env);
      eir_win_api_swap_buffer(&gfx_env);
   }
   EIR_KER_LOG_MESSAGE("stop eir");
   eir_stop(&gfx_env, &sys_env, &snd_env);
   // TODO: display global static array of allocation to see memleaks (only if -D EIR_DEBUG is enabled)
}
