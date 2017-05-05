#ifndef MALLOC_H_
#define MALLOC_H_
#include <stddef.h>

unsigned int *kpmalloc(size_t size, int aligned, unsigned int *physical_addr);

unsigned int *kmalloc_aligned(size_t size, int aligned);

unsigned int kmalloc(size_t size);

#endif
