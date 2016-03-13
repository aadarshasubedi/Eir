#pragma once

#include "../graphics/eir_gfx_rect_proxy.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_gfx_rect_proxy_t * rect_proxy;
} eir_gme_aabb_primitive_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_aabb_primitive_component_t,
	eir_gme_aabb_primitive_component_array_t
	);