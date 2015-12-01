#pragma once

#include "eir_player_state.h"

void eir_gme_proceed_player_move(
   eir_gme_player_state_t * player_state,
   eir_mth_vec2_t * velocity,
   double elapsed_time
   );
