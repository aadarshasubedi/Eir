#pragma once

#include "../kernel/eir_array_macro.h"

typedef struct
{
   float x;
   float y;
} eir_mth_vec2_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_mth_vec2_t, eir_mth_vec2_array_t);

typedef struct
{
   float x;
   float y;
   float z;
} eir_mth_vec3_t;
