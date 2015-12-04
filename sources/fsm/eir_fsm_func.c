#include "../eir.h"
#include "../kernel/eir_env.h"

/********************************************************
 *
 * LOCAL FUNCTIONS
 *
 *******************************************************/

static void eir_fsm_init_state_machine(eir_fsm_state_machine_t * state_machine)
{
   if (state_machine)
   {
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

static eir_fsm_env_t * eir_fsm_get_env(eir_env_t * env)
{
   eir_fsm_env_t * fsm_env = 0;

   if (env && env->private)
   {
      eir_ker_env_t * env_struct = (eir_ker_env_t *)(env->private);

      if (env_struct)
      {
	 fsm_env = &env_struct->fsm_env;
      }
   }
   return fsm_env;
}

static eir_fsm_state_machine_t * eir_fsm_get_state_machine(eir_env_t * env, eir_handle_t handle)
{
   eir_fsm_state_machine_t * state_machine = 0;
   eir_fsm_env_t * fsm_env = eir_fsm_get_env(env);

   if (fsm_env)
   {
      EIR_KER_GET_ARRAY_ITEM(fsm_env->state_machines, handle, state_machine);
   }
   return state_machine;
}

static eir_fsm_state_t * eir_fsm_get_state(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle
   )
{
   eir_fsm_state_t * state = 0;
   eir_fsm_state_machine_t * state_machine = eir_fsm_get_state_machine(env, state_machine_handle);

   if (state_machine)
   {
      EIR_KER_GET_ARRAY_ITEM(state_machine->states, state_handle, state);
   }
   return state;
}

static void eir_fsm_release_state(eir_fsm_state_t * state)
{
   eir_fsm_init_state(state);
}

static void eir_fsm_release_state_machine(eir_fsm_state_machine_t * state_machine)
{
   if (state_machine)
   {
      EIR_KER_RELEASE_ARRAY_AND_DATA(state_machine->states, eir_fsm_release_state);
      eir_fsm_init_state_machine(state_machine);
   }
}

/********************************************************
 *
 * EIR INTERNAL FUNCTIONS (eir_fsm_func.h)
 *
 *******************************************************/

void eir_fsm_init_env(eir_fsm_env_t * env, size_t max_state_machine_count)
{
   if (env)
   {
      EIR_KER_INIT_ARRAY_AND_DATA(
	 eir_fsm_state_machine_t,
	 env->state_machines,
	 max_state_machine_count, 
	 eir_fsm_init_state_machine
	 );
      env->curr_state_machine = 0;
   }
}

void eir_fsm_run_state_machine(eir_fsm_state_machine_t * state_machine)
{
   if (state_machine)
   {
      state_machine->curr_state = state_machine->begin_state;
   }
}

void eir_fsm_update_state_machine(eir_fsm_state_machine_t * state_machine)
{
   if (
      state_machine &&
      state_machine->curr_state &&
      state_machine->curr_state != state_machine->end_state
      )
   {
      for (int state_index = 0; state_index < EIR_FSM_MAX_STATE_OUT; ++state_index)
      {
	 eir_fsm_state_t * state = state_machine->curr_state->out_states[state_index];

	 if (state && state->validate && state->validate())
	 {
	    state_machine->curr_state = state;
	    break;
	 }
      }
      if (state_machine->curr_state && state_machine->curr_state->update)
      {
	 state_machine->curr_state->update();
      }
   }
}

void eir_fsm_release_env(eir_fsm_env_t * env)
{
   if (env)
   {
      EIR_KER_RELEASE_ARRAY_AND_DATA(env->state_machines, eir_fsm_release_state_machine);
      env->curr_state_machine = 0;
   }
}

/********************************************************
 *
 * EIR EXTERNAL FUNCTIONS (eir.h)
 *
 *******************************************************/

eir_handle_t eir_fsm_create_state_machine(eir_env_t * env, size_t max_state_count)
{
   eir_handle_t handle = EIR_INVALID_HANDLE;
   eir_fsm_env_t * fsm_env = eir_fsm_get_env(env);
   eir_fsm_state_machine_t * state_machine = 0;

   if (fsm_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(fsm_env->state_machines, state_machine, handle);
   }
   if (state_machine)
   {
      EIR_KER_INIT_ARRAY_AND_DATA(
	 eir_fsm_state_t,
	 state_machine->states,
	 max_state_count,
	 eir_fsm_init_state
	 );
   }
   return handle;
}

eir_handle_t eir_fsm_create_state(eir_env_t * env, eir_handle_t state_machine_handle)
{
   eir_handle_t handle = EIR_INVALID_HANDLE;
   eir_fsm_state_machine_t * state_machine = eir_fsm_get_state_machine(env, state_machine_handle);
   eir_fsm_state_t * state = 0;

   if (state_machine)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(state_machine->states, state, handle);
   }
   return handle;
}

bool eir_fsm_set_state_validate_func(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle,
   eir_fsm_validate_state_t validate_func
   )
{
   eir_fsm_state_t * state = eir_fsm_get_state(env, state_machine_handle, state_handle);

   if (state)
   {
      state->validate = validate_func;
   }
   return state;
}

bool eir_fsm_set_state_update_func(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle,
   eir_fsm_update_state_t update_func
   )
{
   eir_fsm_state_t * state = eir_fsm_get_state(env, state_machine_handle, state_handle);

   if (state)
   {
      state->update = update_func;
   }
   return state;
}

bool eir_fsm_add_state_transition(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t in_state_handle,
   eir_handle_t out_state_handle
   )
{
   bool result = false;
   eir_fsm_state_t * in_state = eir_fsm_get_state(env, state_machine_handle, in_state_handle);
   eir_fsm_state_t * out_state = eir_fsm_get_state(env, state_machine_handle, out_state_handle);

   if (in_state && out_state)
   {
      for (int state_index = 0; state_index < EIR_FSM_MAX_STATE_OUT; ++state_index)
      {
	 if (!in_state->out_states[state_index])
	 {
	    in_state->out_states[state_index] = out_state;
	    result = true;
	    break;
	 }
      }
   }
   return result;
}

bool eir_fsm_set_begin_state(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle
   )
{
   eir_fsm_state_machine_t * state_machine = eir_fsm_get_state_machine(env, state_machine_handle);
   eir_fsm_state_t * state = eir_fsm_get_state(env, state_machine_handle, state_handle);

   if (state_machine && state)
   {
      state_machine->begin_state = state;
   }
   return state_machine && state;
}

bool eir_fsm_set_end_state(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle
   )
{
   eir_fsm_state_machine_t * state_machine = eir_fsm_get_state_machine(env, state_machine_handle);
   eir_fsm_state_t * state = eir_fsm_get_state(env, state_machine_handle, state_handle);

   if (state_machine && state)
   {
      state_machine->end_state = state;
   }
   return state_machine && state;
}

bool eir_fsm_set_active_state_machine(eir_env_t * env, eir_handle_t state_machine_handle)
{
   eir_fsm_env_t * fsm_env = eir_fsm_get_env(env);
   eir_fsm_state_machine_t * state_machine = eir_fsm_get_state_machine(env, state_machine_handle);

   if (fsm_env)
   {
      fsm_env->curr_state_machine = state_machine;
   }
   return fsm_env;
}
