#include "eir_win_api_func.h"
#include "eir_sys_defines.h"

#define EIR_SYS_PAD_MAX_ABS_VALUE 32767
#define EIR_SYS_PAD_MIN_ABS_VALUE 32768
#define EIR_SYS_PAD_DEAD_ZONE_THRESHOLD 8000

static void eir_sys_process_button_state(eir_button_state_t * button, bool pressed)
{
   button->pressed = pressed;
}

static void eir_sys_process_keyboard_event(eir_input_controller_t * controller, SDL_Event * sdl_event)
{
   if (controller)
   {
      bool pressed = (sdl_event->type == SDL_KEYDOWN);

      if (sdl_event->key.keysym.sym == SDLK_LEFT)
      {
	 eir_sys_process_button_state(&controller->buttons[EIR_MOVE_LEFT_BUTTON_INDEX], pressed);
      }
      if (sdl_event->key.keysym.sym == SDLK_RIGHT)
      {
	 eir_sys_process_button_state(&controller->buttons[EIR_MOVE_RIGHT_BUTTON_INDEX], pressed);
      }
      if (sdl_event->key.keysym.sym == SDLK_UP)
      {
	 eir_sys_process_button_state(&controller->buttons[EIR_MOVE_UP_BUTTON_INDEX], pressed);
      }
      if (sdl_event->key.keysym.sym == SDLK_DOWN)
      {
	 eir_sys_process_button_state(&controller->buttons[EIR_MOVE_DOWN_BUTTON_INDEX], pressed);
      }
      if (sdl_event->key.keysym.sym == SDLK_SPACE)
      {
	 eir_sys_process_button_state(&controller->buttons[EIR_JUMP_BUTTON_INDEX], pressed);
      }
   }
}

static float eir_sys_process_stick_value(int value, int dead_zone_threshold)
{
   float result = 0;

   if (value < -dead_zone_threshold)
   {
      result = (float)(value + dead_zone_threshold) / (EIR_SYS_JOYSTICK_MIN_ABS_VALUE - dead_zone_threshold);
   }
   else if (value > dead_zone_threshold)
   {
      result = (float)(value - dead_zone_threshold) / (EIR_SYS_JOYSTICK_MAX_ABS_VALUE - dead_zone_threshold);
   }
   return result;
}

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

void eir_sys_win_api_release()
{
   EIR_KER_LOG_MESSAGE("release SDL");
   SDL_Quit();
}

void eir_sys_win_api_create_window(eir_sys_env_t * sys_env, int width, int height)
{
   EIR_KER_LOG_MESSAGE("create window");
   sys_env->window = SDL_CreateWindow(
      "eir",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      width,
      height,
      SDL_WINDOW_OPENGL
      );
   sys_env->context = SDL_GL_CreateContext(sys_env->window);
}

void eir_sys_win_api_swap_buffer(eir_sys_env_t * sys_env)
{
   SDL_GL_SwapWindow(sys_env->window);
}

bool eir_sys_win_api_poll_all_events(eir_gme_env_t * gme_env, eir_sys_env_t * sys_env)
{
   if (!gme_env || !sys_env)
   {
      return false;
   }

   SDL_Event sdl_event;
   eir_input_controller_buffer_t * kbrd = &gme_env->input_controllers[EIR_KEYBOARD_CONTROLLER_INDEX];
   eir_input_controller_t * new_kbrd = &kbrd->controllers[1];

   SDL_JoystickEventState(SDL_DISABLE);

   while (SDL_PollEvent(&sdl_event))
   {
      if (sdl_event.type == SDL_KEYUP || sdl_event.type == SDL_KEYDOWN)
      {
	 if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
	 {
	    return false;
	 }
	 else
	 {
	    eir_sys_process_keyboard_event(new_kbrd, &sdl_event);
	 }
      }
   }

   SDL_JoystickUpdate();

   for (int index = 0; index < EIR_TOTAL_INPUT_CONTROLLER; ++index)
   {
      eir_input_controller_buffer_t * pad = &gme_env->input_controllers[index + 1];
      eir_input_controller_t * new_pad = &pad->controllers[1];
      eir_sys_pad_handle_t pad_handle = sys_env->pad_handles[index];

      if (pad_handle == EIR_SYS_INVALID_PAD_HANDLE)
      {
	 continue;
      }
      new_pad->is_connected = true;

      new_pad->left_stick_value_x =
	 eir_sys_process_stick_value(
	    SDL_JoystickGetAxis(pad_handle, 0),
	    EIR_SYS_PAD_DEAD_ZONE_THRESHOLD
	    );
      new_pad->left_stick_value_y =
	 eir_sys_process_stick_value(
	    SDL_JoystickGetAxis(pad_handle, 1),
	    EIR_SYS_PAD_DEAD_ZONE_THRESHOLD
	    );

      if (new_pad->left_stick_value_x != 0.0f || new_pad->left_stick_value_y != 0.0f)
      {
	 new_pad->is_analog = true;
      }
      else
      {
	 new_pad->is_analog = false;
      }
      
      if (SDL_JoystickGetButton(pad_handle, 13))
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_LEFT_BUTTON_INDEX], true);
      }
      else
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_LEFT_BUTTON_INDEX], false);
      }

      if (SDL_JoystickGetButton(pad_handle, 14))
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_RIGHT_BUTTON_INDEX], true);
      }
      else
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_RIGHT_BUTTON_INDEX], false);
      }

      if (SDL_JoystickGetButton(pad_handle, 11))
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_UP_BUTTON_INDEX], true);
      }
      else
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_UP_BUTTON_INDEX], false);
      }

      if (SDL_JoystickGetButton(pad_handle, 12))
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_DOWN_BUTTON_INDEX], true);
      }
      else
      {
	 eir_sys_process_button_state(&new_pad->buttons[EIR_MOVE_DOWN_BUTTON_INDEX], false);
      }
      if (SDL_JoystickGetButton(pad_handle, 0))
      {
         eir_sys_process_button_state(&new_pad->buttons[EIR_JUMP_BUTTON_INDEX], true);
      }
      else
      {
         eir_sys_process_button_state(&new_pad->buttons[EIR_JUMP_BUTTON_INDEX], false);
      }
   }
   return true;
}

void eir_sys_win_api_destroy_window(eir_sys_env_t * sys_env)
{
   SDL_GL_DeleteContext(sys_env->context);
}
