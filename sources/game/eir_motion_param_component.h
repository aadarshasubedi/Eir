#pragma once

#include "../physics/eir_motion_param.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_phy_motion_param_t data;
} eir_gme_motion_param_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_motion_param_component_t, eir_gme_motion_param_component_array_t)
