#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * This is how we deal with engine created object in user code section
 */

typedef int eir_handle_t;
#define EIR_INVALID_HANDLE -1

/**
 * For event management
 */

typedef enum
{
   eir_event_type_unknown,
   eir_event_type_window,
   eir_event_type_keyboard,
   eir_event_type_pad
} eir_event_type_t;

typedef enum
{
   eir_window_event_type_unknown,
   eir_window_event_type_close
} eir_window_event_type_t;

typedef enum
{
   eir_keyboard_event_type_unknown,
   eir_keyboard_event_type_key_down,
   eir_keyboard_event_type_key_up
} eir_keyboard_event_type_t;

typedef enum
{
   eir_keyboard_key_unknown,
   eir_keyboard_key_esc,
   eir_keyboard_key_left,
   eir_keyboard_key_right,
   eir_keyboard_key_up,
   eir_keyboard_key_down
} eir_keyboard_key_t;

typedef struct
{
   eir_window_event_type_t type;
} eir_window_event_t;

typedef struct
{
   eir_keyboard_event_type_t type;
   eir_keyboard_key_t key;
} eir_keyboard_event_t;

typedef struct
{
   float x_axis_value;
   float y_axis_value;
   int pad_index;
} eir_pad_event_t;

typedef struct
{
   eir_event_type_t type;
   union
   {
      eir_window_event_t window_event;
      eir_keyboard_event_t keyboard_event;
      eir_pad_event_t pad_event;
   };
} eir_event_t;

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

// ALLOCATION MODIFICATION. MUST BE CALLED BEFORE ENV CREATION
void eir_set_allocate_func(eir_allocate_t allocate_func);
void eir_set_free_func(eir_free_t free_func);

// ENV CREATION
eir_env_t * eir_create_env();

// FINITE STATE MACHINE CREATION
typedef bool (*eir_fsm_validate_state_t)();
typedef bool (*eir_fsm_validate_state_by_event_t)(const eir_event_t * event);
typedef void (*eir_fsm_update_state_t)();

void eir_fsm_set_max_state_machine_count(eir_env_t * env, size_t max_state_machine_count);
eir_handle_t eir_fsm_create_state_machine(eir_env_t * env, size_t max_state_count);
eir_handle_t eir_fsm_create_state(eir_env_t * env, eir_handle_t state_machine_handle);
bool eir_fsm_set_state_validate_func(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle,
   eir_fsm_validate_state_t validate_func
   );
bool eir_fsm_set_state_validate_by_event_func(
   eir_env_t * env,
   eir_handle_t state_machine_handle,
   eir_handle_t state_handle,
   eir_fsm_validate_state_by_event_t validate_func
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

// SPRITE MANAGEMENT
eir_handle_t eir_gfx_create_texture(const char * img_filename, bool must_invert_img);
eir_handle_t eir_gfx_create_sprite(
   eir_env_t * env,
   eir_handle_t texture_handle,
   int texture_x_offset,
   int texture_y_offset,
   int texture_width_offset,
   int texture_height_offset
   );

// ENTITY MANAGEMENT
eir_handle_t eir_ecs_create_entity(eir_env_t * env);
bool eir_ecs_set_entity_position(eir_env_t * env, int x, int y);
bool eir_ecs_set_entity_size(eir_env_t * env, int width, int height);
bool eir_ecs_set_entity_sprite(eir_env_t * env, eir_handle_t sprite_handle);

// RUN GAME LOOP
void eir_run(eir_env_t * env);

// CALL AT THE END
void eir_destroy_env(eir_env_t * env);
