#pragma once

/******************************************************************
 * TODO:
 *
 *      - add simple jump
 *      - implement animated sprite
 *      - use sprite size for entity when assigned sprite ref component
 *      - resolve TODO in code
 *      - remove hard coded value in camera position code
 *
 *****************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include "kernel/eir_array_macro.h"

/**
 * This is how we deal with engine created object in user code section
 */

typedef int eir_handle_t;
#define EIR_INVALID_HANDLE -1
EIR_KER_DEFINE_ARRAY_STRUCT(eir_handle_t, eir_handle_array_t);

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
#define EIR_JUMP_BUTTON_INDEX 4
#define EIR_TOTAL_INPUT_BUTTON_COUNT 5

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
 * Eir interfaces functions
 */

/* ALLOCATION MODIFICATION  ------------------- */
/* MUST BE CALLED BEFORE EVERYTHING ELSE */

void eir_set_allocate_func(eir_allocate_t allocate_func);
void eir_set_free_func(eir_free_t free_func);

/* ENV CREATION ------------------------------- */

eir_env_t * eir_create_env(int width, int height);

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

bool eir_gme_set_entity_position(eir_gme_world_t * world, eir_gme_entity_t entity, int x, int y);
bool eir_gme_set_entity_size(eir_gme_world_t * world, eir_gme_entity_t entity, int width, int height);
bool eir_gme_set_entity_sprite_ref(eir_gme_world_t * world, eir_gme_entity_t entity, eir_gfx_sprite_ref_t * sprite_ref);
bool eir_gme_set_entity_color(eir_gme_world_t * world, eir_gme_entity_t entity, float r, float g, float b, float a);
bool eir_gme_set_entity_acceleration(eir_gme_world_t * world, eir_gme_entity_t entity, float ax, float ay, float speed, float friction);
bool eir_gme_set_entity_aabb(eir_gme_world_t * world, eir_gme_entity_t entity, float x, float y, float width, float height);
bool eir_gme_set_world_entity_camera(eir_gme_world_t * world, eir_gme_entity_t entity, float win_scale);
bool eir_gme_set_world_entity_active_camera(eir_gme_world_t * world, eir_gme_entity_t entity);
bool eir_gme_set_world_entity_physic(eir_gme_world_t * world, eir_gme_entity_t entity, float weight);

/* ACCESSORS ---------------------------------- */

eir_input_controller_buffer_t * eir_get_input_controller_buffer(eir_env_t * env, int controller_index);
double eir_get_current_time(eir_env_t * env);

/* RUN EIR ENGINE ----------------------------- */

void eir_run(eir_env_t * env);

/* WORLD AND ENTITY MANAGEMENT ---------------- */

void eir_destroy_env(eir_env_t * env);

/* DEBUG -------------------------------------- */

void eir_display_mem_leaks();
