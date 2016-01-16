#pragma once

/******************************************************************
 * TODO:
 *
 * - BUGS
 *   - fix rect display
 *
 * - FEATURES
 *
 *****************************************************************/

#include <stdbool.h>
#include <stddef.h>

/**
 * This is how we deal with engine created object in user code section
 */

typedef int eir_handle_t;
#define EIR_INVALID_HANDLE -1

/**
 * input controller info
 */

typedef struct
{
   bool pressed;
} eir_button_state_t;

#define EIR_MOVE_RIGHT_BUTTON_INDEX 0
#define EIR_MOVE_LEFT_BUTTON_INDEX 1
#define EIR_MOVE_DOWN_BUTTON_INDEX 2
#define EIR_MOVE_UP_BUTTON_INDEX 3
#define EIR_TOTAL_INPUT_BUTTON_COUNT 4

typedef struct
{
   bool is_connected;
   bool is_analog;
   float left_stick_value_x;
   float left_stick_value_y;
   eir_button_state_t buttons[EIR_TOTAL_INPUT_BUTTON_COUNT];
} eir_input_controller_t;

#define EIR_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT 2

typedef struct
{
   eir_input_controller_t controllers[EIR_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT];
} eir_input_controller_buffer_t;

#define EIR_KEYBOARD_CONTROLLER_INDEX 0

// three player can play at the same time and index 0 is for keyboard
#define EIR_TOTAL_INPUT_CONTROLLER 4

/**
 * global env management
 */

typedef struct
{
   void * private;
} eir_env_t;

/**
 * allocation function definition for overriding
 */

typedef void * (*eir_allocate_t)(size_t item_size, size_t item_count);
typedef void (*eir_free_t)(void * ptr);

/**
 * Eir interfaces functions
 */

/* ALLOCATION MODIFICATION  ------------------- */
/* MUST BE CALLED BEFORE EVERYTHING ELSE */

void eir_set_allocate_func(eir_allocate_t allocate_func);
void eir_set_free_func(eir_free_t free_func);

/* ENV CREATION ------------------------------- */

eir_env_t * eir_create_env(int width, int height);

/* FINITE STATE MACHINE CREATION -------------- */

typedef bool (*eir_fsm_validate_state_t)(void * user_data);
typedef void (*eir_fsm_update_state_t)(void * user_data);

void eir_fsm_set_max_state_machine_count(eir_env_t * env, size_t max_state_machine_count);
eir_handle_t eir_fsm_create_state_machine(eir_env_t * env, size_t max_state_count, void * user_data);
eir_handle_t eir_fsm_create_state(eir_env_t * env, eir_handle_t state_machine_handle);
bool eir_fsm_set_state_validate_func(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle,
   eir_fsm_validate_state_t validate_func
   );
bool eir_fsm_set_state_update_func(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle,
   eir_fsm_update_state_t update_func
   );
bool eir_fsm_add_state_transition(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t in_state_handle,
   eir_handle_t out_state_handle
   );
bool eir_fsm_set_begin_state(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle
   );
bool eir_fsm_set_end_state(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle
   );
bool eir_fsm_set_active_state_machine(eir_env_t * env, eir_handle_t state_machine_handle);

/* IMAGE MANAGEMENT --------------------------- */

void eir_gfx_set_max_image_count(eir_env_t * env, size_t max_count);
eir_handle_t eir_gfx_load_image(eir_env_t * env, const char * img_filename, bool must_invert_img);
void eir_gfx_set_max_texture_count(eir_env_t * env, size_t max_count);
eir_handle_t eir_gfx_create_texture(eir_env_t * env, eir_handle_t img_handle);

/* SPRITE MANAGEMENT -------------------------- */

void eir_gfx_set_max_sprite_ref_count(eir_env_t * env, size_t max_count);
eir_handle_t eir_gfx_create_sprite_ref(
   eir_env_t * env,
   eir_handle_t texture_handle,
   int img_x_offset,
   int img_y_offset,
   int img_width_offset,
   int img_height_offset
   );

/* WORLD AND ENTITY MANAGEMENT ---------------- */

void eir_gme_set_max_world_count(eir_env_t * env, size_t max_count);
eir_handle_t eir_gme_create_world(eir_env_t * env, size_t max_entity_count);
void eir_gme_set_curr_world(eir_env_t * env, eir_handle_t world_handle);
eir_handle_t eir_gme_create_world_entity(eir_env_t * env, eir_handle_t world_handle);
bool eir_gme_set_world_entity_position(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   int x,
   int y
   );
bool eir_gme_set_world_entity_size(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   int width,
   int height
   );
bool eir_gme_set_world_entity_sprite_ref(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   eir_handle_t sprite_ref_handle
   );
bool eir_gme_set_world_entity_color(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float r,
   float g,
   float b,
   float a
   );
bool eir_gme_set_world_entity_acceleration(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float x_acceleration,
   float y_acceleration,
   float speed_factor,
   float friction_factor
   );
// WARNING: call eir_gme_set_world_entity_position before
// eir_gme_set_world_entity_aabb if you want to use entity
// relative position for aabb !
// call eir_gme_set_world_entity_aabb before
// eir_gme_set_world_position if you want absolute position
// for aabb !
bool eir_gme_set_world_entity_aabb(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float x,
   float y,
   float width,
   float height
   );
bool eir_gme_set_world_entity_following_camera(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle,
   float cam_win_scale
   );
bool eir_gme_set_world_entity_active_camera(
   eir_env_t * env,
   eir_handle_t world_handle,
   eir_handle_t entity_handle
   );

/* ACCESSORS ---------------------------------- */

eir_input_controller_buffer_t * eir_get_input_controller_buffer(eir_env_t * env, int controller_index);

/* RUN EIR ENGINE ----------------------------- */

void eir_run(eir_env_t * env);

/* WORLD AND ENTITY MANAGEMENT ---------------- */

void eir_destroy_env(eir_env_t * env);

/* DEBUG -------------------------------------- */

void eir_display_mem_leaks();
