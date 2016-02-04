#pragma once

#include "../graphics/eir_sprite_ref.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_gfx_sprite_ref_t * ptr;
} eir_gme_sprite_ref_component_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gme_sprite_ref_component_t, eir_gme_sprite_ref_component_array_t)
