#include <paging.h>
#include <system.h>
#include <tty.h>

extern unsigned int placement;

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
        free_frame(addr / FRAME_SIZE);
    }
}

unsigned int find_free_frame(void) {
    unsigned int start = currFrame;
    do {
        if(!check_frame(currFrame)) {
            return currFrame;
        }
        currFrame = currFrame < numFrames ? currFrame + 1 : 0;
    } while(currFrame != start); 
}

struct page_table_entry *get_page(unsigned int addr, struct page_directory *dir) {
    addr /= FRAME_SIZE;
    int page_index = addr % TABLE_SIZE;
    int table_index = addr / TABLE_SIZE;
    if(!dir->tables[table_index]) {
        unsigned int page_t;
        dir->tables[table_index] = (struct page_table *)kpmalloc(sizeof(struct page_table), FRAME_SIZE, &page_t);

        dir->physTable[table_index] = (page_t << 20)| 0x7; 
    } return &dir->tables[table_index]->entries[page_index]; 
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

void map_address(struct page_directory *dir, unsigned int phys, unsigned int virt) {
    unsigned short dir_index = virt >> 22;
    if(!dir->tables[dir_index]) {
        //dir->tables[dir_index] = (struct page_table *)make_page_table();
        dir->physTable[dir_index] = (unsigned int)dir->tables | 0x7;
    }
    struct page_table *table = dir->tables[dir_index];
    int i;
    for(i = 0; i<TABLE_SIZE; i++) {
        table->entries[i].frame_address = phys >> 12;
        table->entries[i].present = 1;
        phys += FRAME_SIZE;
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
    unsigned int end_map = 0x10000000;
    numFrames = end_map / FRAME_SIZE;
    frames = (unsigned char *)kmalloc(numFrames / 8);
    memset(frames, 0, numFrames / 8);

    kernel_directory = (struct page_directory *)kmalloc_aligned(sizeof(struct page_directory), FRAME_SIZE);
    memset(kernel_directory, 0, sizeof(struct page_directory));

    unsigned int i = 0;
    while(i < placement) {
        allocate_frame(get_page(i, kernel_directory), 1, 1);
        //4K pages
        i+=FRAME_SIZE;
    }
    //curr_directory = copy(kernel_directory);
    set_page_directory(&kernel_directory->physTable);
    enable_paging();
}



