#include "../eir.h"
#include "../kernel/eir_ker_env.h"

/********************************************************
 *
 * LOCAL FUNCTIONS
 *
 *******************************************************/

static void eir_fsm_init_state_machine(eir_fsm_state_machine_t * state_machine)
{
   if (state_machine)
   {
      state_machine->user_data = 0;
      state_machine->begin_state = 0;
      state_machine->end_state = 0;
      state_machine->curr_state = 0;
      state_machine->states.capacity = 0;
      state_machine->states.used = 0;
      state_machine->states.data = 0;
   }
}

static void eir_fsm_init_state(eir_fsm_state_t * state)
{
   if (state)
   {
      state->validate = 0;
      state->update = 0;
      for (int state_index = 0; state_index < EIR_FSM_MAX_STATE_OUT; ++state_index)
      {
	 state->out_states[state_index] = 0;
      }
   }
}

static void eir_fsm_release_state(eir_fsm_state_t * state)
{
   eir_fsm_init_state(state);
}

static void eir_fsm_release_state_machine(eir_fsm_state_machine_t * state_machine)
{
   if (state_machine)
   {
      EIR_KER_FREE_ARRAY_BIS(state_machine->states, eir_fsm_release_state);
      eir_fsm_init_state_machine(state_machine);
   }
}

/********************************************************
 *
 * GLOBAL FUNCTIONs
 *
 *******************************************************/

void eir_fsm_init_env(eir_fsm_env_t * env)
{
   EIR_KER_LOG_MESSAGE("init fsm env");
   if (env)
   {
      EIR_KER_INIT_ARRAY(env->state_machines);
   }
}

void eir_fsm_release_env(eir_fsm_env_t * env)
{
   if (env)
   {
      EIR_KER_LOG_MESSAGE("release fsm env");
      EIR_KER_FREE_ARRAY_BIS(env->state_machines, eir_fsm_release_state_machine);
   }
}

void eir_fsm_set_max_state_machine_count(eir_fsm_env_t * env, size_t max_state_machine_count)
{
   if (env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_fsm_state_machine_t,
	 env->state_machines,
	 max_state_machine_count, 
	 eir_fsm_init_state_machine
	 );
   }
}

eir_fsm_state_machine_t * eir_fsm_create_state_machine(eir_fsm_env_t * env, size_t max_state_count, void * user_data)
{
   eir_fsm_state_machine_t * fsm = 0;

   if (env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(env->state_machines, fsm);
   }
   if (fsm)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_fsm_state_t,
	 fsm->states,
	 max_state_count,
	 eir_fsm_init_state
	 );
      fsm->user_data = user_data;
   }
   return fsm;
}

eir_fsm_state_t * eir_fsm_create_state(eir_fsm_state_machine_t * fsm)
{
   eir_fsm_state_t * state = 0;

   if (fsm)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(fsm->states, state);
   }
   return state;
}

void eir_fsm_set_state_validate_func(eir_fsm_state_t * state, eir_fsm_validate_state_t validate_func)
{
   if (state)
   {
      state->validate = validate_func;
   }
}

void eir_fsm_set_state_update_func(eir_fsm_state_t * state, eir_fsm_update_state_t update_func)
{
   if (state)
   {
      state->update = update_func;
   }
}

void eir_fsm_add_state_transition(eir_fsm_state_t * in_state, eir_fsm_state_t * out_state)
{
   if (in_state && out_state)
   {
      int state_index = 0;

      while (state_index < EIR_FSM_MAX_STATE_OUT)
      {
	 if (!in_state->out_states[state_index])
         {
	    in_state->out_states[state_index] = out_state;
            break;
         }
         ++state_index;
      }
      if (state_index == EIR_FSM_MAX_STATE_OUT)
      {
         EIR_KER_LOG_ERROR("cannot add more transition to state, maximum transition (%d) has previously been reeached", EIR_FSM_MAX_STATE_OUT);
      }
   }
}

void eir_fsm_set_begin_state(eir_fsm_state_machine_t * fsm, eir_fsm_state_t * state)
{
   if (fsm && state)
   {
      fsm->begin_state = state;
   }
}

void eir_fsm_set_end_state(eir_fsm_state_machine_t * fsm, eir_fsm_state_t * state)
{
   if (fsm && state)
   {
      fsm->end_state = state;
   }
}

void eir_fsm_run_state_machine(eir_fsm_state_machine_t * fsm)
{
   EIR_KER_LOG_MESSAGE("run state machine");
   if (fsm)
   {
      fsm->curr_state = fsm->begin_state;
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot run null state machine");
   }
}

void eir_fsm_update_state_machine(eir_fsm_state_machine_t * fsm)
{
   if (fsm && fsm->curr_state && (fsm->curr_state != fsm->end_state))
   {
      for (int state_index = 0; state_index < EIR_FSM_MAX_STATE_OUT; ++state_index)
      {
	 eir_fsm_state_t * state = fsm->curr_state->out_states[state_index];

	 if (state && state->validate && state->validate(fsm->user_data))
	 {
	    fsm->curr_state = state;
	    break;
	 }
      }
      if (fsm->curr_state && fsm->curr_state->update)
      {
	 fsm->curr_state->update(fsm->user_data);
      }
   }
}

