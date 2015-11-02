#pragma once

#include "eir_sys_types.h"

typedef struct
{
   eir_sys_joystick_handle_t handle;
   float x_axis_value;
   float y_axis_value;
} eir_sys_joystick_t;
