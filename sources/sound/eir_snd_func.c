#include "eir_snd_func.h"
#include "eir_snd_api_func.h"

void eir_snd_set_sound_capacity(eir_snd_env_t * snd_env, int max_capacity)
{
   if (snd_env)
   {
      EIR_KER_INIT_ARRAY(eir_snd_sound_handle_t, snd_env->sounds, max_capacity);
   }
}

int eir_snd_load_sound_file(eir_snd_env_t * snd_env, const char * filename)
{
   int index = -1;
   eir_snd_sound_handle_t * sound = 0;

   if (snd_env)
   {
      EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(snd_env->sounds, index);
      EIR_KER_GET_ARRAY_ITEM(snd_env->sounds, index, sound);
   }
   if (sound)
   {
      *sound = eir_snd_api_load_sound_file(filename);
   }
   return index;
}

void eir_snd_play_sound(eir_snd_env_t * snd_env, int sound_index)
{
   if (!snd_env)
   {
      return;
   }

   eir_snd_sound_handle_t * sound = 0;

   EIR_KER_GET_ARRAY_ITEM(snd_env->sounds, sound_index, sound);
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
