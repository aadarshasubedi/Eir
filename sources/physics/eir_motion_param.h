#pragma once

#include "../maths/eir_vector.h"

typedef struct
{
   eir_mth_vec2_t velocity;
   eir_mth_vec2_t max_velocity;
   eir_mth_vec2_t acceleration;
} eir_phy_motion_param_t;
