#pragma once

#include "../maths/eir_vector.h"
#include "../physics/eir_motion_param.h"

typedef enum
{
   eir_gme_player_direction_down = 1 << 0,
   eir_gme_player_direction_up = 1 << 1,
   eir_gme_player_direction_left = 1 << 2,
   eir_gme_player_direction_right = 1 << 3
} eir_gme_player_direction_t;

typedef struct
{
   eir_mth_vec2_t position;
   eir_phy_motion_param_t motion_param;
   int pad_index;
   int direction_flags;
} eir_gme_player_state_t;
