#pragma once

#include "eir_gme_input_controller.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_gme_input_controller_buffer_t * input_buffer;
} eir_gme_keyboard_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_keyboard_component_t,
	eir_gme_keyboard_component_array_t
	)