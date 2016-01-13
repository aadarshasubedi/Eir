#pragma once

#include "../eir.h"
#include "../graphics/eir_gfx_env.h" // TODO: use sys env when created
#include "../system/eir_sys_env.h"

bool eir_sys_win_api_init();
void eir_sys_win_api_release();
void eir_sys_win_api_create_window(eir_gfx_env_t * gfx_env, int width, int height); // TODO: use sys env when created
void eir_sys_win_api_swap_buffer(eir_gfx_env_t * gfx_env); // TODO: use sys env when created
bool eir_sys_win_api_poll_all_events(eir_gme_env_t * gme_env, eir_sys_env_t * sys_env);
void eir_sys_win_api_destroy_window(eir_gfx_env_t * gfx_env); // TODO: use sys env when created
