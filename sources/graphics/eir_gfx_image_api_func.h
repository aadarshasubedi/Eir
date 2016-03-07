#pragma once

#include "eir_gfx_image.h"

eir_gfx_image_t * eir_gfx_api_create_image(const char * filename, bool invert_image);
bool eir_gfx_api_load_image(const char * filename, bool invert_image, eir_gfx_image_t * image);
void eir_gfx_api_destroy_image(eir_gfx_image_t * image);
