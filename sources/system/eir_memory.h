#pragma once 

extern void * (*eir_sys_allocate)(size_t item_size, size_t item_count);
extern void (*eir_sys_free)(void * ptr);

void * eir_sys_default_allocate(size_t item_size, size_t item_count);
void eir_sys_default_free(void * ptr);
void * eir_sys_mem_cpy(void * dest, const void * src, size_t n);
