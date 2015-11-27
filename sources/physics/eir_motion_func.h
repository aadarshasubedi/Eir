#pragma once

#include "eir_motion_param.h"

void eir_phy_proceed_euler_integration(
   const eir_mth_vec2_t * old_position,
   const eir_phy_motion_param_t * motion_param,
   double elapsed_time,
   eir_mth_vec2_t * out_new_position
   );
