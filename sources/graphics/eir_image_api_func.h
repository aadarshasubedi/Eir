#pragma once

#include "eir_image.h"

eir_gfx_image_t * eir_gfx_api_create_image(const char * filename, bool invert_image);
void eir_gfx_api_destroy_image(eir_gfx_image_t * image);
