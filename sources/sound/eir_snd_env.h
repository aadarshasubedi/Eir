#pragma once

#include "eir_snd_types.h"
#include "../kernel/eir_array_macro.h"

EIR_KER_DEFINE_ARRAY_STRUCT(eir_snd_sound_handle_t, eir_snd_sound_handle_array_t);

typedef struct
{
   eir_snd_sound_handle_array_t sounds;
} eir_snd_env_t;
