#pragma once

#include "eir_timer.h"
#include "eir_sys_types.h"

typedef struct
{
   eir_sys_timer_t timer;
   eir_sys_pad_handle_t pad_handle;
} eir_sys_env_t;
