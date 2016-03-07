#pragma once

#include "../maths/eir_mth_vector.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_mth_vec2_t velocity;
   eir_mth_vec2_t acceleration;
   float speed_factor;
   float friction_factor;
} eir_phy_motion_param_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_phy_motion_param_t, eir_phy_motion_param_array_t)
