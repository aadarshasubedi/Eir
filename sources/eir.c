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
}

static void eir_stop(eir_gfx_env_t * gfx_env, eir_sys_env_t * sys_env, eir_snd_env_t * snd_env)
{
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
   int sound_index = -1;

   eir_gfx_set_sprite_batch_capacity(&gfx_env, 2);
   eir_snd_set_sound_capacity(&snd_env, 2);
   sprite_batch = eir_gfx_create_empty_sprite_batch(&gfx_env, 2);

   position.x = 0.0f;
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
   color.a = 0.0f;
   eir_gfx_add_sprite_to_batch(&position, &size, &uv_offset, &uv_size, &color, sprite_batch);

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
   eir_gfx_add_sprite_to_batch(&position, &size, &uv_offset, &uv_size, &color, sprite_batch);

   eir_start(&gfx_env, &sys_env);
   if (eir_sys_get_joystick_count() > 0)
   {
      sys_env.joystick.handle = eir_sys_get_joystick(0);
   }
   eir_snd_api_init();

   sound_index = eir_snd_load_sound_file(&snd_env, "../resources/sounds/medium.wav");
   eir_sys_start_timer(&sys_env.timer);

   for (;;)
   {
      eir_sys_update_timer(&sys_env.timer);
      if (!eir_win_api_poll_all_events(&sys_env.joystick))
      {
	 break;
      }
      eir_gfx_api_set_clear_color();
      eir_gfx_api_clear_buffer();
      sprite_batch->sprites.data[0].position.x +=
	 sys_env.joystick.x_axis_value * sys_env.timer.elapsed_time;
      sprite_batch->sprites.data[0].color.a += sys_env.timer.elapsed_time;
      if (sprite_batch->sprites.data[0].color.a > 1.0f)
      {
	 sprite_batch->sprites.data[0].color.a = 0.0f;
      }
      sprite_batch->modified = true;
      if (sys_env.joystick.x_axis_value != 0)
      {
	 eir_snd_play_sound(&snd_env, sound_index);
      }
      eir_gfx_render_all_batches(&gfx_env);
      eir_win_api_swap_buffer(&gfx_env);
   }
   EIR_KER_LOG_MESSAGE("stop eir");
   eir_stop(&gfx_env, &sys_env, &snd_env);
   // TODO: display global static array of allocation to see memleaks (only if -D EIR_DEBUG is enabled)
}
