#pragma once

#include "../kernel/eir_ker_array_macro.h"
#include "eir_gme_position_component.h"
#include "eir_gme_aabb_component.h"
#include "eir_gme_camera.h"
#include "eir_gme_color_component.h"
#include "eir_gme_size_component.h"
#include "eir_gme_physic_component.h"
#include "eir_gme_sprite_component.h"
#include "eir_gme_motion_param_component.h"
#include "eir_gme_direction_component.h"
#include "eir_gme_based_melee_attack_component.h"
#include "eir_gme_state_component.h"
#include "eir_gme_fsm_component.h"
#include "eir_gme_aabb_primitive_component.h"
#include "eir_gme_keyboard_component.h"
#include "eir_gme_pad_component.h"

typedef int eir_gme_entity_flags_t;
typedef int eir_gme_entity_t;

const int EIR_GME_INVALID_ENTITY = -1;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_entity_flags_t,
   eir_gme_entity_flags_array_t
   );

typedef enum
{
   eir_gme_component_type_none = 0,                      // 0
   eir_gme_component_type_sprite = 1 << 0,               // 2
   eir_gme_component_type_position = 1 << 1,             // 4
   eir_gme_component_type_size = 1 << 2,                 // 8
   eir_gme_component_type_color = 1 << 3,                // 16
   eir_gme_component_type_motion_param = 1 << 4,         // 32
   eir_gme_component_type_aabb = 1 << 5,                 // 64
   eir_gme_component_type_physic = 1 << 6,               // 128
	eir_gme_component_type_direction = 1 << 7,            // 256
	eir_gme_component_type_state = 1 << 8,                // 512
   eir_gme_component_type_fsm = 1 << 9,                  // 1024
   eir_gme_component_type_aabb_primitive = 1 << 10,      // 2048
   eir_gme_component_type_keyboard_controller = 1 << 11, // 4096
   eir_gme_component_type_pad_controller = 1 << 12,      // 8192
   // MAX is 1 << 32. So only 32 components type possible with this method

} eir_gme_component_type_t;

typedef enum name
{
   eir_gme_component_type_extended_none = 0,             // 0
   eir_gme_component_type_based_melee_attack = 1 << 0,   // 2
} eir_gme_extended_component_type_t;

typedef struct
{
   eir_gme_entity_flags_t based;
   eir_gme_entity_flags_t extended;  
} eir_gme_component_flags_t;

typedef struct
{
   eir_gme_entity_flags_array_t entities_flags;
   eir_gme_position_component_array_t positions;
   eir_gme_size_component_array_t sizes;
   eir_gme_color_component_array_t colors;
   eir_gme_sprite_component_array_t sprites;
   eir_gme_motion_param_component_array_t motion_params;
   eir_gme_aabb_component_array_t aabbs;
   eir_gme_physic_component_array_t physics;
	eir_gme_direction_component_array_t directions;
	eir_gme_based_melee_attack_component_array_t based_melee_attacks;
	eir_gme_state_component_array_t states;
   eir_gme_fsm_component_array_t fsms;
   eir_gme_aabb_primitive_component_array_t aabb_primitives;
   eir_gme_keyboard_component_array_t keyboards;
   eir_gme_pad_component_array_t pads;
   eir_gme_camera_t camera;
} eir_gme_world_t;

typedef struct
{
   eir_gme_world_t * world;
   eir_gme_entity_t entity;
} eir_gme_entity_proxy_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_world_t, eir_gme_world_array_t)
