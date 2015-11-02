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

static void eir_start(eir_gfx_env_t * gfx_env)
{
   eir_win_api_init();
   eir_win_api_create_window(gfx_env);
   eir_gfx_api_init();
   eir_gfx_api_build_sprite_batch(&gfx_env->sprite_batch); // TODO: remove when batch system up
}

static void eir_stop(eir_gfx_env_t * gfx_env, eir_sys_env_t * sys_env)
{
   eir_snd_api_release();
   eir_sys_close_joystick(sys_env->joystick.handle);
   eir_gfx_api_release_batch(&gfx_env->sprite_batch); // TODO: remove when batch system up
   eir_win_api_destroy_window(gfx_env);
}

void eir_run()
{
   eir_sys_allocate = eir_sys_default_allocate;
   eir_sys_free = eir_sys_default_free;

   eir_gfx_env_t gfx_env;
   eir_sys_env_t sys_env;
   eir_snd_env_t snd_env;
   eir_mth_vec2_t position;
   eir_mth_vec2_t uv_offset;
   eir_mth_vec2_t uv_size;

   gfx_env.sprite_batch.curr_sprites_count = 0;

   position.x = 0.0f;
   position.y = 0.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 0.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   eir_gfx_add_sprite_to_batch(&position, &uv_offset, &uv_size, &gfx_env.sprite_batch);

   position.x = 2.0f;
   position.y = 2.0f;
   uv_offset.x = 0.0f;
   uv_offset.y = 64.0f;
   uv_size.x = 64.0f;
   uv_size.y = 64.0f;
   eir_gfx_add_sprite_to_batch(&position, &uv_offset, &uv_size, &gfx_env.sprite_batch);

   eir_start(&gfx_env);
   if (eir_sys_get_joystick_count() > 0)
   {
      sys_env.joystick.handle = eir_sys_get_joystick(0);
   }
   eir_snd_api_init();
   snd_env.sound_test = eir_snd_api_load_sound_file("../resources/sounds/medium.wav");
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
      gfx_env.sprite_batch.sprites[0].position.x +=
	 sys_env.joystick.x_axis_value * sys_env.timer.elapsed_time;
      if (sys_env.joystick.x_axis_value != 0)
      {
	 eir_snd_api_play_sound(snd_env.sound_test);
      }
      eir_gfx_api_set_buffer_data(&gfx_env.sprite_batch);
      eir_gfx_api_draw_sprite_batch(&gfx_env.sprite_batch); // TODO: use the draw all func when created
      eir_win_api_swap_buffer(&gfx_env);
   }
   eir_snd_api_release_sound(snd_env.sound_test);
   EIR_KER_LOG_MESSAGE("stop eir");
   eir_stop(&gfx_env, &sys_env);
   // TODO: display global static array of allocation to see memleaks (only if -D EIR_DEBUG is enabled)
}
