#pragma once

#include "eir_world.h"

typedef struct
{
   bool pressed;
} eir_gme_button_state_t;

#define EIR_GME_MOVE_RIGHT_BUTTON_INDEX 0
#define EIR_GME_MOVE_LEFT_BUTTON_INDEX 1
#define EIR_GME_MOVE_DOWN_BUTTON_INDEX 2
#define EIR_GME_MOVE_UP_BUTTON_INDEX 3
#define EIR_GME_ACTION_1_BUTTON_INDEX 4
#define EIR_GME_TOTAL_INPUT_BUTTON_COUNT 5

typedef struct
{
   bool is_connected;
   bool is_analog;
   float left_stick_value_x;
   float left_stick_value_y;
   eir_gme_button_state_t buttons[EIR_GME_TOTAL_INPUT_BUTTON_COUNT];
} eir_gme_input_controller_t;

#define EIR_GME_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT 2

typedef struct
{
   eir_gme_input_controller_t controllers[EIR_GME_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT];
} eir_gme_input_controller_buffer_t;

#define EIR_GME_KEYBOARD_CONTROLLER_INDEX 0

// three player can play at the same time and index 0 is for keyboard
#define EIR_GME_TOTAL_INPUT_CONTROLLER 4
EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_world_t, eir_gme_world_array_t)

#define EIR_GME_MAX_INPUT_BUFFER_COUNT 2

typedef struct
{
   eir_gme_world_array_t worlds;
   eir_gme_world_t * curr_world;
   eir_gme_input_controller_buffer_t input_controllers[EIR_GME_TOTAL_INPUT_CONTROLLER];
} eir_gme_env_t;
