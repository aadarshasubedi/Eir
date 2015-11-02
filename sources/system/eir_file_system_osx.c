#include <stdio.h>
#include "eir_file_system.h"
#include "eir_memory.h"
#include "../kernel/eir_log.h"

bool eir_sys_read_file(const char * filename, bool binary, char ** buffer)
{
   bool result = true;
   size_t file_size = 0;
   FILE * file = 0;
   char * buffer_tmp = 0;

   if (binary)
   {
      file = fopen(filename, "rb");
   }
   else
   {
      file = fopen(filename, "r");
   }
   if (0 != file)
   {
      if (0 == fseek(file, 0, SEEK_END))
      {
	 file_size = ftell(file);

	 if (-1 != file_size && 0 == fseek(file, 0, SEEK_SET))
	 {
	    buffer_tmp = (char *)eir_sys_allocate(sizeof(char), file_size + 1);
	    *buffer = buffer_tmp;
	    file_size = fread(*buffer, sizeof(char), file_size, file);

	    if (0 != file_size)
	    {
	       (*buffer)[file_size] = '\0';
	    }
	 }
      }
      fclose(file);
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot read file %s", filename);
      result = false;
   }
   return result;
}
