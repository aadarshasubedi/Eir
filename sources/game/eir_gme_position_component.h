#pragma once

#include "../maths/eir_mth_vector.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_mth_vec2_t position;
} eir_gme_position_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_position_component_t,
   eir_gme_position_component_array_t
   )
