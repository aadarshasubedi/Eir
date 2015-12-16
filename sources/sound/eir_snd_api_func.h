#pragma once

#include "eir_snd_types.h"

void eir_snd_api_init();
void eir_snd_api_release();
eir_snd_api_chunk_t eir_snd_api_create_chunk_from_file(const char * filename);
void eir_snd_api_destroy_chunk(eir_snd_api_chunk_t * chunk);
void eir_snd_api_play_chunk(eir_snd_api_chunk_t * chunk);
