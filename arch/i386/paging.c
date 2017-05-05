#include <paging.h>
#include <system.h>

//TODO refactor this garbage
void set_frame(unsigned int frame_num) {
    frames[frame_num / 8] |= (1 << (frame_num % 8));
}

int check_frame(unsigned int frame_num) {
    return frames[frame_num / 8] & (1 << (frame_num % 8));
}

void free_frame(unsigned int frame_num) {
    frames[frame_num / 8] &= (1 << (frame_num % 8));
}

void free_page(struct page_table_entry *entry) {
    if(entry->frame_address) {
        unsigned int addr = entry->frame_address;
        entry->frame_address = 0;
        entry->present = 0;        
        free_frame(addr / 0x1000);
    }
}

unsigned int find_free_frame(void) {
    unsigned int start = index;
    do {
        if(check_frame(index)) {
            return index;
        }
        index = index < numFrames ? index + 1 : 0;
    } while(index != start); 
}

struct page_table_entry *get_page(unsigned int addr, struct page_directory *dir) {
    addr /= 0x1000;
    int page_index = addr % 0x400;
    int table_index = addr / 0x400;
    if(!dir->tables[table_index]) {
        unsigned int page_t;
        dir->tables[table_index] = (struct page_table *)kpmalloc(sizeof(struct page_table), 0x1000, &page_t);

        /** Zero out the for the first page table entry. */
        memset(dir->tables[table_index], 0, 0x1000);
        dir->physTable[table_index] = page_t | 0x7; 
    }
    return &dir->tables[table_index]->entries[page_index]; 
}

void allocate_frame(struct page_table_entry *page, int ring_zero, int writePrivelege) {
    /** Check if page doesn't already have a frame allocated. */
    if(!page->frame_address) {
        unsigned int f_index = find_free_frame();
        set_frame(f_index);
        page->present = 1;
        page->supervisor = ring_zero ? 1 : 0;
        page->read_write = writePrivelege ? 1 : 0;
        page->frame_address = f_index;
    }
}

//TODO fix this shit
/**
struct page_directory *copy(struct page_directory *dir) {
    struct page_directory *copy = kmalloc_aligned(sizeof(struct page_directory), 4096);
    unsigned int i;
    for(i = 0; i<1024; i++) {
        copy->tables[i] = make_page_table();
        copy->physTable[i] = dir->physTable[i];
        unsigned int j;
        for(j = 0; j<1024; j++) {
            copy->tables[i]->entries[j] = dir->tables[i]->entries[j];
        }
    }
    return copy;
}
*/

void initialize_paging(void) {
    unsigned int end_map = 0x1E84800;
    numFrames = end_map / 0x1000;
    frames = (unsigned char *)kmalloc(numFrames / 8);
    memset(frames, 0, numFrames / 8);

    kernel_directory = (struct page_directory *)kmalloc_aligned(sizeof(struct page_directory), 0x1000);
    memset(kernel_directory, 0, sizeof(struct page_directory));

    unsigned int i;
    while(i < (numFrames * 0x1000)) {
        allocate_frame(get_page(i, kernel_directory), 1, 1);
        //4K pages
        i+=0x1000;
    }
    //curr_directory = copy(kernel_directory);
    set_page_directory(&kernel_directory->physTable);
    enable_paging();
}



