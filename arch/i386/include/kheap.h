#ifndef KHEAP_H_
#define KHEAP_H_

#define PREV_INUSE_BIT 1
#define INUSE_BIT 2

struct heap_t {
    struct wilderness_chunk *wilderness;
    unsigned int start_address;
    unsigned int end_address;
    struct bin_header *smallBins;
    struct big_bin_header *largeBins;
};

struct chunk_header {
    size_t prevSize;
    size_t size;
    struct chunk_header *prev;
    struct chunk_header *next;
};

struct wilderness_chunk {
   size_t size; 
};

struct bin_header {
    struct bin_header *nextBin;
    size_t size;
    //most recently used (preserves locality)
    struct chunk_header *first;
};

struct big_bin_header {
    struct big_bin_header *nextBin;
    size_t lowerLimit;
    size_t upperLimit;
    struct big_bin *first;
}
struct big_bin {
    struct big_bin *smaller;
    struct big_bin *larger;
    //should be least recently used to decrease fragmentation
    struct chunk_header *first;
};

#endif
