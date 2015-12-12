#include <stdio.h>
#include "eir.h"

static bool validate_idle_state_by_event(const eir_event_t * event)
{
   bool result = false;

   if (
      event &&
      eir_event_type_keyboard == event->type &&
      eir_keyboard_event_type_key_up == event->keyboard_event.type &&
      (
	 eir_keyboard_key_left == event->keyboard_event.key ||
	 eir_keyboard_key_right == event->keyboard_event.key ||
	 eir_keyboard_key_up == event->keyboard_event.key ||
	 eir_keyboard_key_down == event->keyboard_event.key
      )
      )
   {
      result = true;
   }
   return result;
}

static void update_idle_state()
{
   // TODO: stop entity
}

static bool validate_move_state_by_event(const eir_event_t * event)
{
   bool result = false;

   if (
      event &&
      eir_event_type_keyboard == event->type &&
      eir_keyboard_event_type_key_down == event->keyboard_event.type &&
      (
	 eir_keyboard_key_left == event->keyboard_event.key ||
	 eir_keyboard_key_right == event->keyboard_event.key ||
	 eir_keyboard_key_up == event->keyboard_event.key ||
	 eir_keyboard_key_down == event->keyboard_event.key
      )
      )
   {
      result = true;
   }
   return result;
}

static void update_move_state()
{
   // TODO: move entity
}

static void init_fsm(eir_env_t * env)
{
   if (!env)
   {
      return;
   }

   eir_fsm_set_max_state_machine_count(env, 2);

   eir_handle_t fsm = eir_fsm_create_state_machine(env, 5);
   eir_handle_t idle_state = eir_fsm_create_state(env, fsm);
   eir_handle_t move_state = eir_fsm_create_state(env, fsm);
   eir_handle_t end_state = eir_fsm_create_state(env, fsm);

   eir_fsm_set_begin_state(env, fsm, idle_state);
   eir_fsm_set_end_state(env, fsm, end_state);
   eir_fsm_set_state_validate_by_event_func(env, fsm, idle_state, validate_idle_state_by_event);
   eir_fsm_set_state_validate_by_event_func(env, fsm, move_state, validate_move_state_by_event);
   eir_fsm_set_state_update_func(env, fsm, idle_state, update_idle_state);
   eir_fsm_set_state_update_func(env, fsm, move_state, update_move_state);
   eir_fsm_add_state_transition(env, fsm, idle_state, move_state);
   eir_fsm_add_state_transition(env, fsm, move_state, idle_state);
   eir_fsm_set_active_state_machine(env, fsm);
}

static void init_gfx(eir_env_t * env)
{
   eir_gfx_set_max_image_count(env, 10);
   eir_gfx_set_max_sprite_ref_count(env, 10);

   eir_handle_t placeholder_img = eir_gfx_load_image(
      env,
      "../resources/images/placeholder_atlas.png",
      true
      );
   
}

int main()
{
   eir_env_t * env = 0;
   env = eir_create_env();
   init_fsm(env);
   init_gfx(env);
   eir_run(env);
   eir_destroy_env(env);
   return 0;
}
