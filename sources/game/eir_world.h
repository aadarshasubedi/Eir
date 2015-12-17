#pragma once

#include "../kernel/eir_array_macro.h"
#include "../maths/eir_vector.h"
#include "../eir_internal.h"

typedef int eir_gme_entity_t;
#define EIR_GME_INVALID_ENTITY 0

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_entity_t, eir_gme_entity_array_t)

typedef enum
{
   eir_gme_component_type_none = 0,
   eir_gme_component_type_sprite = 1 << 0,
   eir_gme_component_type_position = 1 << 1,
   eir_gme_component_type_size = 1 << 2
} eir_gme_component_type;

typedef struct
{
   eir_gme_entity_array_t entities;
   eir_mth_vec2_array_t positions;
   eir_mth_vec2_array_t sizes;
   eir_handle_array_t sprite_ref_handles;
} eir_gme_world_t;
