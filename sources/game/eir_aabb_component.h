#pragma once

#include "../physics/eir_aabb.h"
#include "../graphics/eir_rect.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   float x_offset;
   float y_offset;
   float width;
   float height;
   eir_gfx_rect_t * rect;
} eir_gme_aabb_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_aabb_component_t, eir_gme_aabb_component_array_t)
