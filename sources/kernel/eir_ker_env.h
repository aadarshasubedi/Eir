#pragma once

#include "../system/eir_event_callback.h"
#include "../system/eir_sys_env.h"
#include "../graphics/eir_gfx_env.h"
#include "../sound/eir_snd_env.h"
#include "../game/eir_gme_env.h"
#include "../fsm/eir_fsm_env.h"

typedef struct
{
   eir_gfx_env_t gfx_env;
   eir_sys_env_t sys_env;
   eir_snd_env_t snd_env;
   eir_gme_env_t gme_env;
   eir_fsm_env_t fsm_env;
   eir_sys_event_callback_t event_callback;
} eir_ker_env_t;
