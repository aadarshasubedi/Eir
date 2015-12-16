#pragma once

#include "../eir_internal.h"
#include "eir_ecs_types.h"
#include "../maths/eir_vector.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_ecs_entity_t, eir_ecs_entity_array_t);

typedef struct
{
   eir_ecs_entity_array_t entities;
   eir_mth_vec2_array_t positions;
   eir_mth_vec2_array_t sizes;
   eir_handle_array_t sprite_ref_handles;
} eir_ecs_env_t;
