#pragma once

#include "../kernel/eir_env.h"

void eir_fsm_init_env(eir_fsm_env_t * env, size_t max_state_machine_count);
void eir_fsm_run_state_machine(eir_fsm_state_machine_t * state_machine);
void eir_fsm_update_state_machine(eir_fsm_state_machine_t * state_machine);
void eir_fsm_release_env(eir_fsm_env_t * env);
