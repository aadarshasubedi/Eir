#pragma once

#include "../kernel/eir_array_macro.h"
#include "eir_position_component.h"
#include "eir_aabb_component.h"
#include "eir_camera_component.h"
#include "eir_color_component.h"
#include "eir_size_component.h"
#include "eir_physic_component.h"
#include "eir_sprite_ref_component.h"
#include "eir_motion_param_component.h"
#include "eir_direction_component.h"

typedef int eir_gme_entity_flags_t;
typedef int eir_gme_entity_t;
#define EIR_GME_INVALID_ENTITY -1

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_entity_flags_t, eir_gme_entity_flags_array_t)

typedef enum
{
   eir_gme_component_type_none = 0,
   eir_gme_component_type_sprite = 1 << 0,
   eir_gme_component_type_position = 1 << 1,
   eir_gme_component_type_size = 1 << 2,
   eir_gme_component_type_color = 1 << 3,
   eir_gme_component_type_motion_param = 1 << 4,
   eir_gme_component_type_aabb = 1 << 5,
   eir_gme_component_type_camera = 1 << 6,
   eir_gme_component_type_physic = 1 << 7,
	eir_gme_component_type_direction = 1 << 8
} eir_gme_component_type;

typedef struct
{
   eir_gme_entity_flags_array_t entities_flags;

   eir_gme_position_component_array_t positions;
   eir_gme_size_component_array_t sizes;
   eir_gme_color_component_array_t colors;
   eir_gme_sprite_ref_component_array_t sprite_refs;
   eir_gme_motion_param_component_array_t motion_params;
   eir_gme_aabb_component_array_t aabbs;
   eir_gme_camera_component_array_t cameras;
   eir_gme_physic_component_array_t physics;
	eir_gme_direction_component_array_t directions;

   eir_gme_camera_component_t * curr_camera;
} eir_gme_world_t;
