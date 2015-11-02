#pragma once

#include "eir_sys_types.h"

int eir_sys_get_joystick_count();
eir_sys_joystick_handle_t eir_sys_get_joystick(int joystick_index);
void eir_sys_close_joystick(eir_sys_joystick_handle_t handle);
