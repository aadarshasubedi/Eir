#pragma once

/**
 * This is how we deal with engine created object in user code section
 */

typedef int eir_handle_t;
#define EIR_INVALID_HANDLE -1

/**
 * Event management
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

typedef void (*eir_event_callback_t)(const eir_event_t * event);

/**
 * Eir environment
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

void eir_set_allocate_func(eir_allocate_t allocate_func); // optional
void eir_set_free_func(eir_free_t free_func); // optional
eir_env_t * eir_create_env();
void eir_set_event_callback(eir_env_t * env, eir_event_callback_t event_callback); // optional ?
void eir_run(eir_env_t * env);
void eir_destroy_env(eir_env_t * env);
