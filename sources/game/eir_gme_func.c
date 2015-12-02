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

/*

static void eir_proceed_player_move(
   eir_gme_player_state_t * player_state,
   eir_sys_env_t * sys_env,
   double elapsed_time
   )
{
   if (!player_state || !sys_env)
   {
      return;
   }

   eir_mth_vec2_t velocity;

   velocity.x = sys_env->joystick.x_axis_value;
   velocity.y = sys_env->joystick.y_axis_value;
   // TODO: check keyboard too if player 1 use keyboard instead of pad
   
   eir_gme_proceed_player_move(player_state, &velocity, elapsed_time);
}

eir_proceed_player_move(&gme_env->player_1_state, sys_env, sys_env->timer.elapsed_time);

 */
