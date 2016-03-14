#pragma once

#include "eir_gme_world.h"
#include "eir_gme_input_controller.h"

#define EIR_GME_KEYBOARD_CONTROLLER_INDEX 0

// three player can play at the same time and index 0 is for keyboard
#define EIR_GME_TOTAL_INPUT_CONTROLLER 4

#define EIR_GME_MAX_INPUT_BUFFER_COUNT 2

typedef struct
{
   eir_gme_world_array_t worlds;
   eir_gme_world_t * curr_world;
   eir_gme_input_controller_buffer_t input_controllers[EIR_GME_TOTAL_INPUT_CONTROLLER];
} eir_gme_env_t;
