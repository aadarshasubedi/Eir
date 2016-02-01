#pragma once

#include "eir_fsm_env.h"


void eir_fsm_init_env(eir_fsm_env_t * env);
void eir_fsm_release_env(eir_fsm_env_t * env);
void eir_fsm_set_max_state_machine_count(eir_fsm_env_t * env, size_t max_state_machine_count);
eir_fsm_state_machine_t * eir_fsm_create_state_machine(eir_fsm_env_t * env, size_t max_state_count, void * user_data);
eir_fsm_state_t * eir_fsm_create_state(eir_fsm_state_machine_t * fsm);
void eir_fsm_set_state_validate_func(eir_fsm_state_t * state, eir_fsm_validate_state_t validate_func);
void eir_fsm_set_state_update_func(eir_fsm_state_t * state, eir_fsm_update_state_t update_func);
void eir_fsm_add_state_transition(eir_fsm_state_t * in_state, eir_fsm_state_t * out_state);
void eir_fsm_set_begin_state(eir_fsm_state_machine_t * fsm, eir_fsm_state_t * state);
void eir_fsm_set_end_state(eir_fsm_state_machine_t * fsm, eir_fsm_state_t * state);
void eir_fsm_run_state_machine(eir_fsm_state_machine_t * fsm);
void eir_fsm_update_state_machine(eir_fsm_state_machine_t * fsm);
