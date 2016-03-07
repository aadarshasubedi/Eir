#pragma once

#include "eir_snd_sound.h"
#include "../kernel/eir_ker_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_snd_sound_t, eir_snd_sound_array_t);

typedef struct
{
   eir_snd_sound_array_t sounds;
} eir_snd_env_t;
