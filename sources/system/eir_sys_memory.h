#pragma once 

#include <stddef.h>

typedef void * (*eir_allocate_t)(size_t item_size, size_t item_count);
typedef void (*eir_free_t)(void * ptr);

extern eir_allocate_t eir_sys_allocate;
extern eir_free_t eir_sys_free;

void * eir_sys_default_allocate(size_t item_size, size_t item_count);
void eir_sys_default_free(void * ptr);
void * eir_sys_mem_cpy(void * dest, const void * src, size_t n);

#ifdef EIR_LOG_MEMORY

void * eir_sys_debug_allocate(
   const char * func_name, 
   const char * ptr_name,
   int file_line,
   size_t item_size,
   size_t item_count
   );
void eir_sys_debug_free(void * item_ptr);
void eir_sys_debug_log_allocation();

#define EIR_SYS_MAX_BUFFER_COUNT 256
extern int eir_sys_allocated_elem_count;

typedef struct eir_sys_allocated_elem_s
{
   void * ptr;
   char owner_info[EIR_SYS_MAX_BUFFER_COUNT];
   struct eir_sys_allocated_elem_s * previous;
   struct eir_sys_allocated_elem_s * next;
} eir_sys_allocated_elem_t;

extern eir_sys_allocated_elem_t * eir_sys_allocated_elem_curr;

#define EIR_SYS_ALLOC(item_ptr, item_size, item_count)			\
   item_ptr = eir_sys_debug_allocate(__func__, #item_ptr, __LINE__, item_size, item_count);

#define EIR_SYS_FREE(item_ptr)			\
   eir_sys_debug_free(item_ptr);

#define EIR_SYS_LOG_ALLOCATED_ELEM		\
   eir_sys_debug_log_allocation();

#else

#define EIR_SYS_ALLOC(item_ptr, item_size, item_count)	\
   item_ptr = eir_sys_allocate(item_size, item_count);

#define EIR_SYS_FREE(item_ptr)			\
   eir_sys_free(item_ptr);

#define EIR_SYS_LOG_ALLOCATED_ELEM

#endif
