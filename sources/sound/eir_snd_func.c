#include "eir_snd_func.h"
#include "eir_snd_api_func.h"

void eir_snd_set_sound_capacity(eir_snd_env_t * snd_env, int max_capacity)
{
   if (snd_env)
   {
      EIR_KER_INIT_ARRAY(eir_snd_sound_handle_t, snd_env->sounds, max_capacity);
   }
}

eir_handle_t eir_snd_load_sound_file(eir_snd_env_t * snd_env, const char * filename)
{
   eir_handle_t sound_handle = -1;
   eir_snd_sound_handle_t * sound = 0;

   if (snd_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(snd_env->sounds, sound, sound_handle);
   }
   if (sound)
   {
      *sound = eir_snd_api_load_sound_file(filename);
   }
   return sound_handle;
}

void eir_snd_play_sound(eir_snd_env_t * snd_env, eir_handle_t sound_handle)
{
   if (!snd_env)
   {
      return;
   }

   eir_snd_sound_handle_t * sound = 0;

   EIR_KER_GET_ARRAY_ITEM(snd_env->sounds, sound_handle, sound);
   if (sound)
   {
      eir_snd_api_play_sound(*sound);
   }
}

void eir_snd_release_all_sounds(eir_snd_env_t * snd_env)
{
   if (!snd_env)
   {
      return;
   }

   eir_snd_sound_handle_t * sound = 0;

   for (int index = 0; index < snd_env->sounds.used; ++index)
   {
      EIR_KER_GET_ARRAY_ITEM(snd_env->sounds, index, sound);
      if (sound)
      {
	 eir_snd_api_release_sound(*sound);
      }
   }
}
