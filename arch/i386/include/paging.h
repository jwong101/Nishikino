#ifndef PAGING_H_
#define PAGING_H_
#include <malloc.h>

#define PAGE_DIRECTORY_ADDRESS 0x9C000

#define FRAME_SIZE 0x1000

#define GET_BIT(ADDRESS) (ADDRESS / FRAME_SIZE % sizeof(frames[0]))
#define GET_BYTE_INDEX(ADDRESS) (ADDRESS / FRAME_SIZE / sizeof(frames[0]))
#define TOTAL_FRAMES

static char *frames;
static unsigned int numFrames;

static unsigned int index;

struct page_table_entry {
    unsigned char present : 1;
    unsigned char read_write : 1;
    unsigned char supervisor : 1;
    unsigned char write_through_cache : 1;
    unsigned char cache_disable : 1;
    unsigned char accessed : 1;
    unsigned char dirty : 1;
    unsigned char zero : 1;
    unsigned char global : 1;
    unsigned char available : 3;
    unsigned int frame_address : 20;
};

struct page_table {
    struct page_table_entry entries[1024];
} __attribute__((aligned(4096)));

struct page_directory {
    struct page_table *tables[1024];
    unsigned int physTable[1024];
} __attribute__((aligned(4096)));

struct page_directory *kernel_directory;
/** Used for switching the task paging directories. */
struct page_directory *curr_directory;

void set_frame(unsigned int frame_num);

void free_page(struct page_table_entry *entry);

unsigned int find_free_frame(void);

int check_frame(unsigned int frame_num);

void allocate_frame(struct page_table_entry *page, int ring_zero, int writePrivelege);

struct page_table_entry *get_page(unsigned int addr, struct page_directory *dir);

void free_frame(unsigned int frame_num);

struct page_directory *copy(struct page_directory *dir);

void initialize_paging(void);

#endif
