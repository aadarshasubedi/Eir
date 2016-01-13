#pragma once

#include "../physics/eir_aabb.h"
#include "../graphics/eir_rect.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_mth_vec2_t target_last_pos;
   eir_mth_vec2_t cam_pos;
   eir_phy_aabb_t cam_win_aabb;
   eir_phy_aabb_t * target_aabb;
   eir_gfx_rect_t * cam_win_rect;
} eir_gme_camera_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_camera_component_t, eir_gme_camera_component_array_t)
