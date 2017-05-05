#include <malloc.h>

extern unsigned int end;
unsigned int placement = (unsigned int)&end;

unsigned int *kpmalloc(size_t size, int aligned, unsigned int *physical_addr) {
    if(aligned) {
        size += placement % aligned;
    }
    unsigned int tmp = placement;
    if(physical_addr) {
        *physical_addr = tmp;       
    }
    placement += size;
    return (unsigned int *)tmp; 
}

unsigned int *kmalloc_aligned(size_t size, int aligned) {
    return kpmalloc(size, aligned, 0);
}

unsigned int kmalloc(size_t size) {
    return kpmalloc(size, 0, 0);
}
