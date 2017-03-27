#include <system.h>
#include <interrupt.h>
#include <tty.h>

// 101
#define TASK_GATE_OFFSET ( 1 << 2 & 0x01 )
// 110 000
#define INTERRUPT_GATE_OFFSET ( 3 << 4 )
// 111 000
#define TRAP_GATE_OFFSET ( 7 << 3 )
#define SET_SIZE_BIT ( 1 << 12 )

extern void load_idt();

const char *exception_messages[] = {
    "Division by Zero"
};

struct idt_entry {
    unsigned short lower_offset;
    unsigned short segment;
    unsigned char middle_reserve;
    unsigned char flags;
    unsigned short upper_offset;

} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base_address;
} __attribute__((packed));

struct idt_entry idt_tab[256];
struct idt_ptr idt_ptr;

void insert_idt_entry(unsigned char index, unsigned int base, unsigned short segment, unsigned char flags) {
    idt_tab[index].lower_offset = base & 0xFFFF;
    idt_tab[index].upper_offset = (base >> 16) & 0xFFFF;
    idt_tab[index].middle_reserve = 0;
    idt_tab[index].segment = segment;
    idt_tab[index].flags = flags;
}

void init_idt(void) {
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base_address = &idt_tab;
    memset(&idt_tab, 0, sizeof(struct idt_entry) * 256);

    load_idt(&idt_ptr);

    setup_interrupt_service_routines();
}

void setup_interrupt_service_routines(void) {
    // 1 00 01110
    // 1 00 0 0110
    insert_idt_entry(0, (unsigned int)isr0, 0x08, 0x8E);
    //insert_idt_entry(1, isr1, 0x08, 0x8E);
    /**
    insert_idt_entry(2, (unsigned)isr2, 0x08, 0x8E);
    insert_idt_entry(3, (unsigned)isr3, 0x08, 0x8E);
    
    insert_idt_entry(4, (unsigned)isr4, 0x08, 0x8E);
    insert_idt_entry(5, (unsigned)isr5, 0x08, 0x8E);
    insert_idt_entry(6, (unsigned)isr6, 0x08, 0x8E);
    insert_idt_entry(7, (unsigned)isr7, 0x08, 0x8E);
    insert_idt_entry(8, (unsigned)isr8, 0x08, 0x8E);
    insert_idt_entry(9, (unsigned)isr9, 0x08, 0x8E);
    insert_idt_entry(10, (unsigned)isr10, 0x08, 0x8E);
    insert_idt_entry(11, (unsigned)isr11, 0x08, 0x8E);
    insert_idt_entry(12, (unsigned)isr12, 0x08, 0x8E);
    insert_idt_entry(13, (unsigned)isr13, 0x08, 0x8E);
    insert_idt_entry(14, (unsigned)isr14, 0x08, 0x8E);
    insert_idt_entry(15, (unsigned)isr15, 0x08, 0x8E);
    insert_idt_entry(16, (unsigned)isr16, 0x08, 0x8E);
    insert_idt_entry(17, (unsigned)isr17, 0x08, 0x8E);
    insert_idt_entry(18, (unsigned)isr18, 0x08, 0x8E);
    insert_idt_entry(19, (unsigned)isr19, 0x08, 0x8E);
    insert_idt_entry(20, (unsigned)isr20, 0x08, 0x8E);
    insert_idt_entry(21, (unsigned)isr21, 0x08, 0x8E);
    insert_idt_entry(22, (unsigned)isr22, 0x08, 0x8E);
    insert_idt_entry(23, (unsigned)isr23, 0x08, 0x8E);
    insert_idt_entry(24, (unsigned)isr24, 0x08, 0x8E);
    insert_idt_entry(25, (unsigned)isr25, 0x08, 0x8E);
    insert_idt_entry(26, (unsigned)isr26, 0x08, 0x8E);
    insert_idt_entry(27, (unsigned)isr27, 0x08, 0x8E);
    insert_idt_entry(28, (unsigned)isr28, 0x08, 0x8E);
    insert_idt_entry(29, (unsigned)isr29, 0x08, 0x8E);
    insert_idt_entry(30, (unsigned)isr30, 0x08, 0x8E);
    insert_idt_entry(31, (unsigned)isr31, 0x08, 0x8E);
    */
}

void handle_exception(struct reg_states *regs) {
    if(regs->interrupt_num < 32) {
        kprintf(exception_messages[regs->interrupt_num]);
        kprintf("You fucked up\n");
        while(1);
    }
}
