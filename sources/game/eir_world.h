#pragma once

#include "../kernel/eir_array_macro.h"
#include "../maths/eir_vector.h"
#include "../eir_internal.h"
#include "../graphics/eir_color.h"
#include "../physics/eir_motion_param.h"
#include "eir_position_component.h"
#include "eir_aabb_component.h"
#include "eir_camera_component.h"
#include "eir_color_component.h"
#include "eir_size_component.h"

typedef int eir_gme_entity_t;
#define EIR_GME_INVALID_ENTITY 0

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_entity_t, eir_gme_entity_array_t)

typedef enum
{
   eir_gme_component_type_none = 0,
   eir_gme_component_type_sprite = 1 << 0,
   eir_gme_component_type_position = 1 << 1,
   eir_gme_component_type_size = 1 << 2,
   eir_gme_component_type_color = 1 << 3,
   eir_gme_component_type_motion_param = 1 << 4,
   eir_gme_component_type_aabb = 1 << 5,
   eir_gme_component_type_camera = 1 << 6
} eir_gme_component_type;

// TODO: use component struct with batch linked variable for color and size
// for batch update !
typedef struct
{
   eir_gme_entity_array_t entities;
   eir_gme_position_component_array_t positions;
   eir_mth_vec2_array_t sizes;
   eir_gfx_color_array_t colors;
   eir_handle_array_t sprite_ref_handles;
   eir_phy_motion_param_array_t motion_params;
   eir_gme_aabb_component_array_t aabbs;
   eir_gme_camera_component_array_t cameras;
   eir_gme_camera_component_t * curr_camera;
} eir_gme_world_t;
