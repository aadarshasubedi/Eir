#pragma once

#include "../eir.h"
#include "../kernel/eir_array_macro.h"

#define EIR_FSM_MAX_STATE_OUT 10

typedef bool (*eir_fsm_validate_state_t)(void * user_data);
typedef void (*eir_fsm_update_state_t)(void * user_data);

typedef struct eir_fsm_state_s
{
   eir_fsm_validate_state_t validate;
   eir_fsm_update_state_t update;
   struct eir_fsm_state_s * out_states[EIR_FSM_MAX_STATE_OUT];
} eir_fsm_state_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_fsm_state_t, eir_fsm_state_array_t);

typedef struct
{
   void * user_data;
   eir_fsm_state_t * begin_state;
   eir_fsm_state_t * end_state;
   eir_fsm_state_t * curr_state;
   eir_fsm_state_array_t states;
} eir_fsm_state_machine_t;
