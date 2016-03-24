#pragma once

#include "../graphics/eir_gfx_color.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_gfx_color_t color;
	bool modified;
} eir_gme_color_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_color_component_t,
	eir_gme_color_component_array_t
	);
