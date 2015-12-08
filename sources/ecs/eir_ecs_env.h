#pragma once

#include "eir_ecs_types.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_ecs_entity_t, eir_ecs_entity_array_t);

typedef struct
{
   eir_ecs_entity_array_t entities;
} eir_ecs_env_t;
