#include "eir_image_api_func.h"
#include "eir_gfx_types.h" // TODO: replace by special include for image api when done
#include "../system/eir_memory.h"

eir_gfx_image_t * eir_gfx_api_create_image(const char * filename, bool invert_image)
{
   eir_gfx_image_t * image = 0;

   EIR_SYS_ALLOC(image, sizeof(eir_gfx_image_t), 1);

   if (image)
   {
      SDL_Surface * sdl_image = IMG_Load(filename);

      if(!sdl_image)
      {
	 EIR_KER_LOG_ERROR("cannot load image from file %s: %s\n", filename, IMG_GetError());
      }
      else
      {
	 if (invert_image)
	 {
	    eir_gfx_invert_image(sdl_image->pitch, sdl_image->h, sdl_image->pixels);
	 }

	 size_t buffer_length = sdl_image->h * sdl_image->pitch;

	 EIR_SYS_ALLOC(image->pixels, sizeof(char), buffer_length);
	 image->width = sdl_image->w;
	 image->height = sdl_image->h;
	 eir_sys_mem_cpy(image->pixels, sdl_image->pixels, buffer_length);
	 SDL_FreeSurface(sdl_image);
      }
   }
   return image;
}

void eir_gfx_api_destroy_image(eir_gfx_image_t * image)
{
   if (image)
   {
      if (image->pixels)
      {
	 EIR_SYS_FREE(image->pixels);
      }
      EIR_SYS_FREE(image);
   }
}
