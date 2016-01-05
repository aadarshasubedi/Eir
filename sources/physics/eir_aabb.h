#pragma once

#include "../maths/eir_vector.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_mth_vec2_t position;
   eir_mth_vec2_t size;
} eir_phy_aabb_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_phy_aabb_t, eir_phy_aabb_array_t)
