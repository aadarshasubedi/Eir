#pragma once

#include "../eir.h"

typedef int eir_ecs_entity_t;
typedef eir_handle_t eir_ecs_entity_handle_t;
#define EIR_ECS_INVALID_ENTITY_HANDLE EIR_INVALID_HANDLE

typedef enum
{
   eir_ecs_component_type_none = 0,
   eir_ecs_component_type_sprite = 1 << 0,
   eir_ecs_component_type_physics_controler = 1 << 1,
   eir_ecs_component_type_keyboard_controler = 1 << 2,
   eir_ecs_component_type_pad_controler = 1 << 3
} eir_ecs_component_type;
