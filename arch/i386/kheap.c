#include <kheap.h>


void chunkInUse(struct chunk_header *chunk) {
    return chunk & INUSE_BIT;
}

void prevChunkInUse(struct chunk_header *chunk) {
    return chunk & PREV_INUSE_BIT;
}

void *allocate(size_t size, heap_t *heap) {
    size += sizeof(struct chunk_header);

    //align size to 8 bytes
    size += size % 8;

    if(size <= 256) {
        struct bin_header *small = heap->smallBins;
        while(small->size < size) {
            small = small->nextBin;
        }
        struct chunk_header *chunk = small->first; 
        unlink(chunk);
        //TODO set prevSize
        chunk->size = size;
        return chunk->prev;
    } else {
        struct big_bin_header *big = heap->largeBins;
        unsigned char found = 0;
        while(found || big->upperLimit < size) {
            struct big_bin *it = big->first; 
            struct big_bin *match = findClosest(it, size); 
        }
    }
}


void findMinDiff(struct big_bin *bin, size_t size);
struct big_bin *findClosest(struct big_bin *bin, size_t size) {
    
}

