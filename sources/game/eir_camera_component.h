#pragma once

#include "../physics/eir_aabb.h"
#include "../graphics/eir_rect.h"
#include "../kernel/eir_array_macro.h"
#include "eir_aabb_component.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t prev_position;
   eir_gme_aabb_component_t * target;
   eir_gfx_rect_t * win_rect;
   float win_scale;
   int viewport_w;
   int viewport_h;
} eir_gme_camera_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_camera_component_t, eir_gme_camera_component_array_t)

