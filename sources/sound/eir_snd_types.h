#pragma once

#ifdef EIR_USE_SDL
#include <SDL_mixer.h>
typedef Mix_Chunk * eir_snd_api_chunk_t;
#define EIR_SND_INVALID_API_CHUNK 0;
#endif
