#pragma once

#include "eir_gfx_types.h"
#include "eir_image.h"

typedef struct
{
   eir_gfx_api_texture_handle_t id;
   eir_gfx_image_t * image;
} eir_gfx_texture_t;
