
#include "eir_image_func.h"
#include "../system/eir_memory.h"

bool eir_gfx_invert_image(int pitch, int height, void * pixels)
{
   if (pitch <= 0 || height <= 0 || pixels == 0)
   {
      return false;
   }

   void * row_tmp = eir_sys_allocate(1, pitch);
   int height_div_2 = (int)(height * 0.5f);

   for (int index = 0; index < height_div_2; ++index)
   {
      eir_sys_mem_cpy(
	 (unsigned char *)row_tmp,
	 (unsigned char *)(pixels) + pitch * index,
	 pitch
	 );
      eir_sys_mem_cpy(
	 (unsigned char *)(pixels) + pitch * index,
	 (unsigned char *)(pixels) + pitch * (height - index - 1),
	 pitch
	 );
      eir_sys_mem_cpy(
	 (unsigned char *)(pixels) + pitch * (height - index - 1),
	 row_tmp,
	 pitch
	 );
   }
   eir_sys_free(row_tmp);
   return true;
}
