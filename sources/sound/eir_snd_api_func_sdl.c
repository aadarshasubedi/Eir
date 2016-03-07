#include <SDL_mixer.h>
#include "eir_snd_api_func.h"
#include "../kernel/eir_ker_log.h"

void eir_snd_api_init()
{
   SDL_version compile_version;
   const SDL_version * link_version = Mix_Linked_Version();

   SDL_MIXER_VERSION(&compile_version);
   EIR_KER_LOG_MESSAGE(
      "SDL mixer compile vers: %d.%d.%d",
      compile_version.major,
      compile_version.minor,
      compile_version.patch
      );
   EIR_KER_LOG_MESSAGE(
      "SDL mixer link vers: %d.%d.%d",
      link_version->major,
      link_version->minor,
      link_version->patch
      );

   if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
   {
      EIR_KER_LOG_ERROR("SDL mixer open audio init failed: %s", Mix_GetError());
   }
}

void eir_snd_api_release()
{
   Mix_CloseAudio();
}

eir_snd_api_chunk_t eir_snd_api_create_chunk_from_file(const char * filename)
{
   eir_snd_api_chunk_t chunk = Mix_LoadWAV(filename);
   
   if (!chunk)
   {
      EIR_KER_LOG_ERROR("cannot load sound file %s: %s", filename, Mix_GetError());
   }
   return chunk;
}

void eir_snd_api_destroy_chunk(eir_snd_api_chunk_t * chunk)
{
   Mix_FreeChunk(*chunk);
}

void eir_snd_api_play_chunk(eir_snd_api_chunk_t * chunk)
{
   Mix_PlayChannel(-1, *chunk, 0);
}
