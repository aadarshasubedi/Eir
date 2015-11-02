#pragma once

#include "eir_timer.h"
#include "eir_joystick.h"

typedef struct
{
   eir_sys_timer_t timer;
   eir_sys_joystick_t joystick;
} eir_sys_env_t;
