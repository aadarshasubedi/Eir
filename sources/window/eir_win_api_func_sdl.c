#include "eir_win_api_func.h"
#include "../graphics/eir_gfx_types.h" // TODO: use own include for api include when created
#include "../system/eir_sys_defines.h"

bool eir_win_api_init()
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
void eir_win_api_create_window(eir_gfx_env_t * gfx_env)
{
   EIR_KER_LOG_MESSAGE("create window");
   gfx_env->window = SDL_CreateWindow("eir", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
   gfx_env->context = SDL_GL_CreateContext(gfx_env->window);
}

// TOOD: use window own env when created
void eir_win_api_swap_buffer(eir_gfx_env_t * gfx_env)
{
   SDL_GL_SwapWindow(gfx_env->window);
}

// TODO: pass user function to process events
bool eir_win_api_poll_all_events(eir_sys_joystick_t * joystick)
{
   SDL_Event event;

   while (SDL_PollEvent(&event))
      switch (event.type)
      {
      case SDL_QUIT:
	 return false;
      case SDL_KEYUP:
	 // TODO: let user choose when to quit Eir
	 if (event.key.keysym.sym == SDLK_ESCAPE)
	    return false;
	 break;
      case SDL_JOYAXISMOTION:
      {
	 if (event.jaxis.which == 0 && joystick)
	 {
	    if (event.jaxis.axis == 0)
	    {
	       if (event.jaxis.value > EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  joystick->x_axis_value =
		     (float)event.jaxis.value / (float)EIR_SYS_JOYSTICK_MAX_ABS_VALUE;
	       }
	       else if (event.jaxis.value < -EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  joystick->x_axis_value =
		     (float)event.jaxis.value / (float)EIR_SYS_JOYSTICK_MIN_ABS_VALUE;
	       }
	       else
	       {
		  joystick->x_axis_value = 0.0f;
	       }
	    }
	    else if (event.jaxis.axis == 1) // Y
	    {
	       if (event.jaxis.value > EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  joystick->y_axis_value =
		     (float)event.jaxis.value / (float)EIR_SYS_JOYSTICK_MAX_ABS_VALUE;
	       }
	       else if (event.jaxis.value < -EIR_SYS_JOYSTICK_DEAD_ZONE)
	       {
		  joystick->y_axis_value =
		     (float)event.jaxis.value / (float)EIR_SYS_JOYSTICK_MIN_ABS_VALUE;
	       }
	       else
	       {
		  joystick->x_axis_value = 0.0f;
	       }
	    }
	 }
      }
      break;
      default:
	 break;
      }
   // TODO: call process event callback when available
   return true;
}

void eir_win_api_destroy_window(eir_gfx_env_t * gfx_env)
{
   SDL_GL_DeleteContext(gfx_env->context);
   SDL_Quit();
}
