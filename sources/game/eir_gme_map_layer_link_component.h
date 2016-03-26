#pragma once

#include "eir_gme_entity.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_gme_entity_t map_entity;
   int map_layer_index;
} eir_gme_map_layer_link_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_map_layer_link_component_t,
   eir_gme_map_layer_link_component_array_t
   );
