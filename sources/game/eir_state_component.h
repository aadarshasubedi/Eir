#pragma once

#include "../kernel/eir_array_macro.h"

typedef struct
{
	bool visible;
	bool alive;
} eir_gme_state_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_state_component_t, eir_gme_state_component_array_t)
