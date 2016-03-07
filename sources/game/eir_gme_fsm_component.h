#pragma once

#include "../fsm/eir_fsm_struct.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_fsm_state_machine_t * fsm;
} eir_gme_fsm_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_fsm_component_t,
	eir_gme_fsm_component_array_t
	)
