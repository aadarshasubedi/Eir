#include "eir_snd_func.h"
#include "eir_snd_api_func.h"

static void eir_snd_init_sound(eir_snd_sound_t * sound)
{
   if (sound)
   {
      sound->chunk = EIR_SND_INVALID_API_CHUNK;
   }
}

static void eir_snd_release_sound(eir_snd_sound_t * sound)
{
   if (sound)
   {
      eir_snd_api_destroy_chunk(&sound->chunk);
      eir_snd_init_sound(sound);
   }
}

void eir_snd_init_env(eir_snd_env_t * env)
{
   if (env)
   {
      EIR_KER_INIT_ARRAY(env->sounds);
   }
}

void eir_snd_release_env(eir_snd_env_t * env)
{
   if (env)
   {
      EIR_KER_LOG_MESSAGE("release sound env");
      EIR_KER_FREE_ARRAY_BIS(env->sounds, eir_snd_release_sound);
   }
}

void eir_snd_set_sound_capacity(eir_snd_env_t * snd_env, int max_capacity)
{
   if (snd_env)
   {
      EIR_KER_ALLOCATE_ARRAY_BIS(
	 eir_snd_sound_t,
	 snd_env->sounds,
	 max_capacity,
	 eir_snd_init_sound
	 );
   }
}

eir_handle_t eir_snd_load_sound_file(eir_snd_env_t * snd_env, const char * filename)
{
   eir_handle_t sound_handle = -1;
   eir_snd_sound_t * sound = 0;

   if (snd_env)
   {
      EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(snd_env->sounds, sound, sound_handle);
   }
   if (sound)
   {
      sound->chunk = eir_snd_api_create_chunk_from_file(filename);
   }
   return sound_handle;
}

void eir_snd_play_sound(eir_snd_env_t * snd_env, eir_handle_t sound_handle)
{
   if (snd_env)
   {
      eir_snd_sound_t * sound = 0;

      EIR_KER_GET_ARRAY_ITEM(snd_env->sounds, sound_handle, sound);
      if (sound)
      {
	 eir_snd_api_play_chunk(&sound->chunk);
      }
   }
}
