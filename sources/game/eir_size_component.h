#pragma once

#include "../maths/eir_vector.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_mth_vec2_t initial;
   eir_mth_vec2_t * current;
} eir_gme_size_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_size_component_t, eir_gme_size_component_array_t)
