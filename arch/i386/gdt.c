#include <stdint.h>

/**
 * Structure of the GDT Segment descriptor
 * |           |             |
 * | LIMIT     |  LOWER_BASE | My kernel is 32 bit so these are on separate registers 
 * | BITS 0-15 |  BITS 16-31 | 
 * |           |             |
 * |-----------------------------------------------
 * |             |           |  
 * | MIDDLE_BASE | TYPE      | Descriptor type
 * | BITS 0-7    | BITS 8-11 |  Bit 9 0 = system 
 * |             |           |  1 = code/data
 */

/**
 * clear the old gdt and sets the pointer to the new gdt
 * this should be done in assembly through the lgdtr instruction
 */
extern void gdt_flush();

struct gdt_entry {
    unsigned short limit_low;
    //base is the address
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access; //access byte
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed)); //same shit as below

/**
 * GDT pointer must be 48 bits
 */
struct gdt_ptr {
    unsigned short limit; //lower 16 bits tell size of the gdt
    unsigned int base; //upper 32 bits tell the location in memory
} __attribute__((packed)); //no compiler optimization shenanigans or the full gdt ptr won't be loaded onto the register when lgdtr is called

struct gdt_entry table[3];
struct gdt_ptr ptr;

void gdt_insert_descriptor(int index, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran) {

    table[index].base_low = base & 0xFFFF; //lower 16 bits
    table[index].base_middle = (base >> 16) & 0xFF; //middle 8 bits
    table[index].base_high = (base >> 24) & 0xFF; //upper 8 bits
     
    table[index].limit_low = limit & 0xFFFF; //lower 16 bits of limit
    table[index].granularity = (limit >> 16) & 0x0F;

    table[index].access = access;
    table[index].granularity |= gran & 0xF0;
}

/**
 * Sets up the GDT.
 */
void setup_gdt(void) {
    //null descriptor
    //first section of the GDT is not used by the cpu
    gdt_insert_descriptor(0, 0, 0, 0, 0);

    //code segment
    //4 GByte limit 4kbyte gran
    // 1001 1010 E/R
    gdt_insert_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    //data segment 
    // 1001 0010  R/W bit set with most significant bit says code or data
    gdt_insert_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    ptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    ptr.base = (unsigned int)&table;

    gdt_flush(&ptr);
}

