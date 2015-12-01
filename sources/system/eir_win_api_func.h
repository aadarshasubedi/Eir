#pragma once

#include "../graphics/eir_gfx_env.h" // TODO: use window own env when created
#include "../system/eir_joystick.h"

bool eir_sys_win_api_init();
void eir_sys_win_api_create_window(eir_gfx_env_t * gfx_env); // TODO: use window own env when created
void eir_sys_win_api_swap_buffer(eir_gfx_env_t * gfx_env); // TODO: use window own env when created
bool eir_sys_win_api_poll_all_events(eir_sys_joystick_t * joystick); // TODO: pass user function to process events
void eir_sys_win_api_destroy_window(eir_gfx_env_t * gfx_env); // TODO: use window own env when created
