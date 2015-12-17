#pragma once

#include "eir_world.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_world_t, eir_gme_world_array_t)

typedef struct
{
   eir_gme_world_array_t worlds;
   eir_gme_world_t * curr_world;
} eir_gme_env_t;
