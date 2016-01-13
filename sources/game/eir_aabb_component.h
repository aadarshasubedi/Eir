#pragma once

#include "../physics/eir_aabb.h"
#include "../graphics/eir_rect.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_phy_aabb_t aabb;
   eir_gfx_rect_t * curr_rect;
} eir_gme_aabb_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_aabb_component_t, eir_gme_aabb_component_array_t)
