#include <SDL2/SDL.h>
#include "eir_timer_func.h"
#include "eir_memory.h"

void eir_sys_start_timer(eir_sys_timer_t * timer)
{
   timer->last_time = (double)(SDL_GetTicks()) / 1000.0f;
   timer->elapsed_time = 0.0;
}

void eir_sys_update_timer(eir_sys_timer_t * timer)
{
   double curr_time = (double)(SDL_GetTicks()) / 1000.0f;

   timer->elapsed_time = curr_time - timer->last_time;
   timer->last_time = curr_time;
}
