#pragma once

#include "eir_gfx_types.h"
#include "eir_image.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
   eir_gfx_api_texture_handle_t id;
   const eir_gfx_image_t * image;
} eir_gfx_texture_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_texture_t, eir_gfx_texture_array_t);

