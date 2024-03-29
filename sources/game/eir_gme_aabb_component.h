#pragma once

#include "../physics/eir_phy_aabb.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_phy_aabb_t aabb;
   float x_offset;
   float y_offset;
   bool modified;
} eir_gme_aabb_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
   eir_gme_aabb_component_t,
   eir_gme_aabb_component_array_t
   );
