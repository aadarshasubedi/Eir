#pragma once

#include "../eir.h"
#include "../graphics/eir_gfx_env.h" // TODO: use sys env when created
#include "../system/eir_joystick.h"

bool eir_sys_win_api_init();
void eir_sys_win_api_create_window(eir_gfx_env_t * gfx_env); // TODO: use sys env when created
void eir_sys_win_api_swap_buffer(eir_gfx_env_t * gfx_env); // TODO: use sys env when created
bool eir_sys_win_api_poll_all_events(eir_event_callback_t event_callback, eir_env_t * env);
void eir_sys_win_api_destroy_window(eir_gfx_env_t * gfx_env); // TODO: use sys env when created
