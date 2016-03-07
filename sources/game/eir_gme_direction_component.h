#pragma once

#include "../kernel/eir_ker_array_macro.h"

typedef int eir_gme_direction_t;

const int EIR_GME_DIRECTION_UNKNOWN = -1;
const int EIR_GME_DIRECTION_UP = 0;
const int EIR_GME_DIRECTION_UP_RIGHT = 1;
const int EIR_GME_DIRECTION_RIGHT = 2;
const int EIR_GME_DIRECTION_BOTTOM_RIGHT = 3;
const int EIR_GME_DIRECTION_BOTTOM = 4;
const int EIR_GME_DIRECTION_BOTTOM_LEFT = 5;
const int EIR_GME_DIRECTION_LEFT = 6;
const int EIR_GME_DIRECTION_UP_LEFT = 7;
const int EIR_GME_MAX_DIRECTION_COUNT = 8;
const float EIR_GME_DIRECTION_EPSILON_VALUE = 0.001f;

typedef struct
{
	eir_gme_direction_t direction;
} eir_gme_direction_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_direction_component_t, eir_gme_direction_component_array_t)
