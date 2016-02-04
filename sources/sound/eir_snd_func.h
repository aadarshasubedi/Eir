#pragma once

#include "eir_snd_env.h"

void eir_snd_init_env(eir_snd_env_t * env);
void eir_snd_release_env(eir_snd_env_t * env);
void eir_snd_set_sound_capacity(eir_snd_env_t * snd_env, int max_capacity);
eir_snd_sound_t * eir_snd_load_sound_file(eir_snd_env_t * snd_env, const char * filename);
void eir_snd_play_sound(eir_snd_env_t * snd_env, eir_snd_sound_t * sound);
