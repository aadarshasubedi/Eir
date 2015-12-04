#pragma once

#include "../kernel/eir_array_macro.h"
#include "eir_fsm_struct.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_fsm_state_machine_t, eir_fsm_state_machine_array_t);

typedef struct
{
   eir_fsm_state_machine_array_t state_machines;
   eir_fsm_state_machine_t * curr_state_machine;
} eir_fsm_env_t;
