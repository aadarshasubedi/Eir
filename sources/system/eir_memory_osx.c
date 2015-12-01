#include <stdlib.h>
#include <string.h>
#include "eir_memory.h"

#ifdef EIR_LOG_MEMORY
int eir_sys_allocated_elem_count = 0;
eir_sys_allocated_elem_t * eir_sys_allocated_elem_curr = 0;
eir_sys_allocated_elem_t * eir_sys_allocated_elem_root = 0;
#endif

eir_allocate_t eir_sys_allocate = 0;
eir_free_t eir_sys_free = 0;

void * eir_sys_default_allocate(size_t item_size, size_t item_count)
{
   return malloc(item_size * item_count);
}

void eir_sys_default_free(void * ptr)
{
   if (ptr)
   {
      free(ptr);
      ptr = 0;
   }
}

void * eir_sys_mem_cpy(void * dest, const void * src, size_t n)
{
   return memcpy(dest, src, n);
}

#ifdef EIR_LOG_MEMORY

void * eir_sys_debug_allocate(
   const char * func_name, 
   const char * ptr_name,
   int file_line,
   size_t item_size,
   size_t item_count
   )
{
   void * ptr = eir_sys_allocate(item_size, item_count);

   if (ptr)
   {
      eir_sys_allocated_elem_t * new_elem = eir_sys_allocate(sizeof(eir_sys_allocated_elem_t), 1);

      if (new_elem)
      {
	 new_elem->ptr = ptr;
	 new_elem->previous = 0;
	 new_elem->next = 0;
	 snprintf(
	    new_elem->owner_info,
	    EIR_SYS_MAX_BUFFER_COUNT,
	    "%s(%d):%s, %p",
	    func_name,
	    file_line,
	    ptr_name,
	    ptr
	    );
	 if (eir_sys_allocated_elem_curr)
	 {
	    eir_sys_allocated_elem_curr->next = new_elem;
	    new_elem->previous = eir_sys_allocated_elem_curr;
	 }
	 eir_sys_allocated_elem_curr = new_elem;
      }
   }
   return ptr;
}

void eir_sys_debug_free(void * item_ptr)
{
   if (item_ptr)
   {
      eir_sys_allocated_elem_t * elem_iter = eir_sys_allocated_elem_curr;

      while (elem_iter)
      {
	 if (elem_iter->ptr == item_ptr)
	 {
	    if (elem_iter->previous)
	    {
	       elem_iter->previous->next = elem_iter->next;
	    }
	    if (elem_iter->next)
	    {
	       elem_iter->next->previous = elem_iter->previous;
	    }
	    if (elem_iter == eir_sys_allocated_elem_curr)
	    {
	       eir_sys_allocated_elem_curr = elem_iter->previous;
	    }
	    eir_sys_free(elem_iter);
	    eir_sys_free(item_ptr);
	    return;
	 }
	 elem_iter = elem_iter->previous;
      }
   }
   EIR_KER_LOG_ERROR("no allocated item found for %p", item_ptr);
}

void eir_sys_debug_log_allocation()
{
   eir_sys_allocated_elem_t * elem_iter = eir_sys_allocated_elem_curr;

   if (!elem_iter)
   {
      EIR_KER_LOG_MESSAGE("no allocated item found");
   }
   else
   {
      while (elem_iter)
      {
	 EIR_KER_LOG_MESSAGE("allocated item found: %s", elem_iter->owner_info);
	 elem_iter = elem_iter->previous;
      }
   }
}

#endif
