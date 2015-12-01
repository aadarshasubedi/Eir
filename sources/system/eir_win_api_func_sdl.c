#include "eir_win_api_func.h"
#include "../graphics/eir_gfx_types.h" // TODO: use own include for api include when created
#include "eir_sys_defines.h"

bool eir_sys_win_api_init()
{
   bool result = true;

   EIR_KER_LOG_MESSAGE("init SDL");
   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) == -1)
   {
      EIR_KER_LOG_ERROR("SDL init failed: %s", SDL_GetError());
      result = false;
   }
   else
   {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
   }
   return result;
}

// TOOD: use window own env when created
void eir_sys_win_api_create_window(eir_gfx_env_t * gfx_env)
{
   EIR_KER_LOG_MESSAGE("create window");
   gfx_env->window = SDL_CreateWindow("eir", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
   gfx_env->context = SDL_GL_CreateContext(gfx_env->window);
}

// TOOD: use window own env when created
void eir_sys_win_api_swap_buffer(eir_gfx_env_t * gfx_env)
{
   SDL_GL_SwapWindow(gfx_env->window);
}

bool eir_sys_win_api_poll_all_events(eir_event_callback_t event_callback, eir_env_t * env)
{
   SDL_Event sdl_event;

   while (SDL_PollEvent(&sdl_event))
      switch (sdl_event.type)
      {
      case SDL_QUIT:
	 return false;
      case SDL_KEYUP:
      {
	 eir_event_t event;

	 event.type = eir_event_type_keyboard;
	 event.keyboard_event.type = eir_keyboard_event_type_key_up;
	 if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
	 {
	    event.keyboard_event.key = eir_keyboard_key_esc;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_LEFT)
	 {
	    event.keyboard_event.key = eir_keyboard_key_left;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_RIGHT)
	 {
	    event.keyboard_event.key = eir_keyboard_key_right;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_UP)
	 {
	    event.keyboard_event.key = eir_keyboard_key_up;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_DOWN)
	 {
	    event.keyboard_event.key = eir_keyboard_key_down;
	 }
	 return event_callback(&event, env);
      }
      case SDL_KEYDOWN:
      {
	 eir_event_t event;

	 event.type = eir_event_type_keyboard;
	 event.keyboard_event.type = eir_keyboard_event_type_key_down;
	 if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
	 {
	    event.keyboard_event.key = eir_keyboard_key_esc;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_LEFT)
	 {
	    event.keyboard_event.key = eir_keyboard_key_left;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_RIGHT)
	 {
	    event.keyboard_event.key = eir_keyboard_key_right;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_UP)
	 {
	    event.keyboard_event.key = eir_keyboard_key_up;
	 }
	 if (sdl_event.key.keysym.sym == SDLK_DOWN)
	 {
	    event.keyboard_event.key = eir_keyboard_key_down;
	 }
	 return event_callback(&event, env);
      }
      case SDL_JOYAXISMOTION:
      {
	 if (event_callback)
	 {
	    eir_event_t event;

	    event.type = eir_event_type_pad;
	    event.pad_event.pad_index = sdl_event.jaxis.which;
	    if (sdl_event.jaxis.axis == 0)
	    {
	       if (sdl_event.jaxis.value > EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  event.pad_event.x_axis_value =
		     (float)sdl_event.jaxis.value / (float)EIR_SYS_JOYSTICK_MAX_ABS_VALUE;
	       }
	       else if (sdl_event.jaxis.value < -EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  event.pad_event.x_axis_value =
		     (float)sdl_event.jaxis.value / (float)EIR_SYS_JOYSTICK_MIN_ABS_VALUE;
	       }
	       else
	       {
		  event.pad_event.x_axis_value = 0.0f;
	       }
	    }
	    else if (sdl_event.jaxis.axis == 1) // Y
	    {
	       if (sdl_event.jaxis.value > EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  event.pad_event.y_axis_value =
		     -(float)sdl_event.jaxis.value / (float)EIR_SYS_JOYSTICK_MAX_ABS_VALUE;
	       }
	       else if (sdl_event.jaxis.value < -EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  event.pad_event.y_axis_value =
		     -(float)sdl_event.jaxis.value / (float)EIR_SYS_JOYSTICK_MIN_ABS_VALUE;
	       }
	       else
	       {
		  event.pad_event.y_axis_value = 0.0f;
	       }
	    }
	    return event_callback(&event, env);
	 }
	 break;
      }
      default:
	 break;
      }
   return true;
}

void eir_sys_win_api_destroy_window(eir_gfx_env_t * gfx_env)
{
   SDL_GL_DeleteContext(gfx_env->context);
   SDL_Quit();
}
