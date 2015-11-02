#include <stdlib.h>
#include <string.h>
#include "eir_memory.h"

void * (*eir_sys_allocate)(size_t item_size, size_t item_count);
void (*eir_sys_free)(void * ptr);

void * eir_sys_default_allocate(size_t item_size, size_t item_count)
{
   // TODO: save all allocation in a global static array (only if -D EIR_DEBUG is enabled)
   return malloc(item_size * item_count);
}

void eir_sys_default_free(void * ptr)
{
   if (0 != ptr)
   {
      free(ptr);
   }
}

void * eir_sys_mem_cpy(void * dest, const void * src, size_t n)
{
   // TODO: remove allocation from global static array (only if -D EIR_DEBUG is enabled)
   return memcpy(dest, src, n);
}
