#include <stdio.h>
#include "eir.h"

#define PLACE_HOLDER_IMAGE_PATH "../resources/images/placeholder_atlas.png"
#define PLAYER_FRICTION 10.0f
#define PLAYER_SPEED 80.0f

typedef struct
{
   eir_env_t * env;
   eir_handle_t world_handle;
   eir_handle_t entity_handle;
   eir_input_controller_buffer_t * keyboard_buffer;
   eir_input_controller_buffer_t * pad_buffer;
} player_t;

static bool validate_idle_state(void * user_data)
{
   bool result = false;
   
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      if (
	 player->keyboard_buffer
	 && player->pad_buffer
	 && !player->pad_buffer->controllers[1].is_connected
	 && !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	 && !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	 && !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	 && !player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	 )
      {
	 result = true;
      }
      else if (
	 player->pad_buffer
	 && player->pad_buffer->controllers[1].is_connected
	 && !player->pad_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	 && !player->pad_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	 && !player->pad_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	 && !player->pad_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	 )
      {
	 result = true;
      }
   }
   return result;
}

static bool validate_move_state(void * user_data)
{
   bool result = false;

   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      if (
	 player->keyboard_buffer
	 && player->pad_buffer
	 && !player->pad_buffer->controllers[1].is_connected
	 && (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    || player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    || player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 )
      {
	 result = true;
      }
      else if (
	 player->pad_buffer
	 && player->pad_buffer->controllers[1].is_connected
	 && (
	    player->pad_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 )
      {
	 result = true;
      }
   }
   return result;
}

static void update_idle_state(void * user_data)
{
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      eir_gme_set_world_entity_acceleration(
	 player->env,
	 player->world_handle,
	 player->entity_handle,
	 0.0f,
	 0.0f,
	 PLAYER_SPEED,
	 PLAYER_FRICTION
	 );
   }
}

static void update_move_state(void * user_data)
{
   if (user_data)
   {
      player_t * player = (player_t *)user_data;

      float x_velocity = 0.0f;
      float y_velocity = 0.0f;

      if (player->keyboard_buffer)
      {
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_RIGHT_BUTTON_INDEX].pressed
	    )
	 {
	    x_velocity = 1.0f;
	 }
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_LEFT_BUTTON_INDEX].pressed
	    )
	 {
	    x_velocity = -1.0f;
	 }
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_DOWN_BUTTON_INDEX].pressed
	    )
	 {
	    y_velocity = -1.0f;
	 }
	 if (
	    player->keyboard_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    || player->pad_buffer->controllers[1].buttons[EIR_MOVE_UP_BUTTON_INDEX].pressed
	    )
	 {
	    y_velocity = 1.0f;
	 }
      }

      if (eir_mth_abs(x_velocity) > 0.0f && eir_mth_abs(y_velocity) > 0.0f)
      {
	 x_velocity *= 0.707107f;
	 y_velocity *= 0.707107f;
      }

      eir_gme_set_world_entity_acceleration(
	 player->env,
	 player->world_handle,
	 player->entity_handle,
	 x_velocity,
	 y_velocity,
	 PLAYER_SPEED,
	 PLAYER_FRICTION
	 );
   }
}

int main()
{
   // CREATE ENV

   eir_env_t * env = 0;
   env = eir_create_env();

   // INIT GFX ITEMS CAPACITY

   eir_gfx_set_max_image_count(env, 1);
   eir_gfx_set_max_sprite_ref_count(env, 1);
   eir_gfx_set_max_texture_count(env, 1);

   // LOAD SPRITE

   eir_handle_t ph_atlas_image = eir_gfx_load_image(env, PLACE_HOLDER_IMAGE_PATH, true);
   eir_handle_t ph_texture = eir_gfx_create_texture(env, ph_atlas_image);
   eir_handle_t ph_sprite_ref = eir_gfx_create_sprite_ref(env, ph_texture, 0, 0, 64, 64);

   // INIT WORLD ENTITIES

   eir_gme_set_max_world_count(env, 1);

   eir_handle_t world = eir_gme_create_world(env, 1);
   
   eir_gme_set_curr_world(env, world);

   eir_handle_t entity = eir_gme_create_world_entity(env, world);

   eir_gme_set_world_entity_position(env, world, entity, -10, 0);
   eir_gme_set_world_entity_size(env, world, entity, 2, 2);
   eir_gme_set_world_entity_sprite_ref(env, world, entity, ph_sprite_ref);
   eir_gme_set_world_entity_color(env, world, entity, 0.0f, 1.0f, 0.0f, 0.5f);
   eir_gme_set_world_entity_acceleration(env, world, entity, 0.0f, 0.0f, PLAYER_SPEED, PLAYER_FRICTION);

   // INIT PLAYER USER DATA

   player_t player;

   player.env = env;
   player.world_handle = world;
   player.entity_handle = entity;
   player.keyboard_buffer = eir_get_input_controller_buffer(env, 0);
   player.pad_buffer = eir_get_input_controller_buffer(env, 1);

   // INIT STATE MACHINE

   eir_fsm_set_max_state_machine_count(env, 1);

   eir_handle_t fsm = eir_fsm_create_state_machine(env, 3, &player);
   eir_handle_t idle_state = eir_fsm_create_state(env, fsm);
   eir_handle_t move_state = eir_fsm_create_state(env, fsm);
   eir_handle_t end_state = eir_fsm_create_state(env, fsm);

   eir_fsm_set_begin_state(env, fsm, idle_state);
   eir_fsm_set_end_state(env, fsm, end_state);
   eir_fsm_set_active_state_machine(env, fsm);
   
   eir_fsm_set_state_validate_func(env, fsm, idle_state, validate_idle_state);
   eir_fsm_set_state_validate_func(env, fsm, move_state, validate_move_state);

   eir_fsm_set_state_update_func(env, fsm, idle_state, update_idle_state);
   eir_fsm_set_state_update_func(env, fsm, move_state, update_move_state);

   eir_fsm_add_state_transition(env, fsm, idle_state, move_state);
   eir_fsm_add_state_transition(env, fsm, move_state, idle_state);

   // RUN EIR ENGINE

   eir_run(env);

   // DESTROY ENV

   eir_destroy_env(env);
   eir_display_mem_leaks();

   return 0;
}
