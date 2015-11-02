#pragma once

#include "eir_snd_types.h"

void eir_snd_api_init();
void eir_snd_api_release();
eir_snd_sound_handle_t eir_snd_api_load_sound_file(const char * filename);
void eir_snd_api_release_sound(eir_snd_sound_handle_t handle);
void eir_snd_api_play_sound(eir_snd_sound_handle_t handle);
