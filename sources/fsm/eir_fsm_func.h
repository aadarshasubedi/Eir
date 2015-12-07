#pragma once

#include "../eir.h"
#include "../kernel/eir_env.h"

void eir_fsm_run_state_machine(eir_fsm_env_t * env);
void eir_fsm_update_state_machine(eir_fsm_env_t * env);
void eir_fsm_process_event(eir_env_t * env, const eir_event_t * event);
void eir_fsm_release_env(eir_fsm_env_t * env);
