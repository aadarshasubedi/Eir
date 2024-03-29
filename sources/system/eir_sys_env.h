#pragma once

#include "eir_sys_timer.h"
#include "eir_sys_types.h"

typedef struct
{
   eir_sys_window_handle_t window;
   eir_sys_context_handle_t context;
   eir_sys_timer_t timer;
   eir_sys_pad_handle_t pad_handles[EIR_GME_TOTAL_INPUT_CONTROLLER - 1];
} eir_sys_env_t;
