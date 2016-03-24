#pragma once

#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   float weight;
} eir_gme_physic_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_physic_component_t,
	eir_gme_physic_component_array_t
	);
