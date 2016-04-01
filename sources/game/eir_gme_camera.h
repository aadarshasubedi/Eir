#pragma once

#include "../physics/eir_phy_aabb.h"
#include "eir_gme_aabb_component.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t prev_position;
   eir_gme_aabb_component_t * target;
   eir_phy_aabb_t win_aabb;
   float win_scale;
   int viewport_w;
   int viewport_h;
} eir_gme_camera_t;
