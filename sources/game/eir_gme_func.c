#include "eir_gme_func.h"
#include "../physics/eir_motion_func.h"

void eir_gme_proceed_player_move(
   eir_gme_player_state_t * player_state,
   eir_mth_vec2_t * velocity,
   double elapsed_time)
{
   if (player_state && velocity)
   {
      player_state->motion_param.velocity.x = velocity->x;
      player_state->motion_param.velocity.y = velocity->y;

      eir_mth_vec2_t new_position;
      eir_mth_vec2_t new_velocity;

      eir_phy_proceed_euler_integration(
	 &player_state->position,
	 &player_state->motion_param,
	 elapsed_time,
	 &new_position,
	 &new_velocity
	 );
      player_state->position.x = new_position.x;
      player_state->position.y = new_position.y;
   }
}
