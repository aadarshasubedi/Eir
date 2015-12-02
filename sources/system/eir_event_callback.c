#include "eir_event_callback.h"

bool eir_sys_default_event_callback(const eir_event_t * event, eir_env_t * env)
{
   bool result = true;

   if (event && env)
   {
      if (eir_event_type_keyboard == event->type)
      {
	 if (eir_keyboard_key_esc == event->keyboard_event.key)
	 {
	    result = false;
	 }
      }
   }
   return result;
}
