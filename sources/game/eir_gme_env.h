#pragma once

#include "../eir.h"
#include "eir_world.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_world_t, eir_gme_world_array_t)

#define EIR_GME_MAX_INPUT_BUFFER_COUNT 2

typedef struct
{
   eir_gme_world_array_t worlds;
   eir_gme_world_t * curr_world;
   eir_input_buffer_t input_buffer;
} eir_gme_env_t;
