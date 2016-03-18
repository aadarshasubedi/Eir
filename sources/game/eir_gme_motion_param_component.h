#pragma once

#include "../physics/eir_phy_motion_param.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
	eir_phy_motion_param_t motion_param;
} eir_gme_motion_param_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_motion_param_component_t,
	eir_gme_motion_param_component_array_t
	);

typedef struct
{
	int entity_index;
	float ax;
	float ay;
	float speed;
	float friction;
} eir_gme_motion_param_info_t;

EIR_KER_DEFINE_ARRAY_STRUCT(
	eir_gme_motion_param_info_t,
	eir_gme_motion_param_info_array_t
	);

#define EIR_GME_PARSE_motion_param(buffer, item, bytes_read) \
   sscanf(                                            \
      buffer,                                         \
      "%d|%f|%f|%f|%f|%n",                            \
      &item.entity_index,                             \
      &item.ax,                                       \
      &item.ay,                                       \
      &item.speed,                                    \
      &item.friction,                                 \
      &bytes_read                                     \
      );
