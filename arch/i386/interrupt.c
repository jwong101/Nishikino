#include <system.h>
#include <interrupt.h>
#include <timer.h>
#include <tty.h>

// 101
#define TASK_GATE_OFFSET ( 1 << 2 & 0x01 )
// 110 000
#define INTERRUPT_GATE_OFFSET ( 3 << 4 )
// 111 000
#define TRAP_GATE_OFFSET ( 7 << 3 )
#define SET_SIZE_BIT ( 1 << 12 )

extern void load_idt();

//TODO make the length a variable and wrap this in a struct
//length 22
const char *exception_messages[] = {
    "Division by Zero",
    "Debug Exception", 
    "NMI Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device not available",
    "Double fault",
    "Coprocesor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack segment fault",
    "General Protection", //13
    "Page fault",
    "Reserved",
    "x87 FPU Error",
    "Alignment Check",
    "Machine check",
    "SIMD FP Exception",
    "Virtualization Exception",
    "Reserved"
};

struct idt_entry {
    unsigned short lower_base; //lower 16 bits of address
    unsigned short segment; //segment it is in, typically code
    unsigned char middle_reserve; //filled with 0s
    unsigned char flags;
    unsigned short upper_base; //upper 16 bits of address

} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base_address;
} __attribute__((packed));

//I've learned my lesson with encpasulation
static struct idt_entry idt_tab[256];

//array of irq routines
static void (*irq_routines[16])(struct reg_states *regs) = 
   {0, 0, 0, 0, 0, 0, 0, 0, 
    0 ,0 ,0, 0, 0, 0, 0, 0};


struct idt_ptr idt_ptr;

void insert_idt_entry(unsigned char index, unsigned int base, unsigned short segment, unsigned char flags) {
    idt_tab[index].lower_base = base & 0xFFFF;
    idt_tab[index].upper_base = (base >> 16) & 0xFFFF;
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
    setup_irqs();

}

//TODO add an interrupt to the IDT that catches an exception for array out of bounds
void clear_routine(unsigned char index) {
    irq_routines[index] = 0;
}

void install_irq_entry(unsigned char index, void (*irq_routine)(struct reg_states *regs)) {
    irq_routines[index] = irq_routine;
}

void reprogram_pic(void) {
    //unsigned char mmask = read_port(MASTER_DATA);
    //io_wait();
    //unsigned char smask = read_port(SLAVE_DATA);
    //io_wait();

    write_port(MASTER_COMMAND, INIT_PIC + ICW4);
    io_wait();
    write_port(SLAVE_COMMAND, INIT_PIC + ICW4);
    io_wait();
    //send PIC updated offset to where the master and slave start in the IDT
    write_port(MASTER_DATA, MASTER_OFFSET);
    io_wait();
    write_port(SLAVE_DATA, SLAVE_OFFSET);
    io_wait();

    write_port(MASTER_DATA, 4);
    io_wait();
    write_port(SLAVE_DATA, 2);
    io_wait();
    
    write_port(MASTER_DATA, PIC_8086_MODE);
    io_wait();
    write_port(SLAVE_DATA, PIC_8086_MODE);
    io_wait();

    //write_port(MASTER_DATA, mmask);
    //write_port(SLAVE_DATA, smask);
    //write_port(MASTER_DATA, 0);
    //io_wait();
    //write_port(SLAVE_DATA, 0);
    //io_wait();
}

void setup_interrupt_service_routines(void) {
    // Task Gate 101
    // Trap Gate 111
    // P Priv ring        Size 1 = 32 bit  Interrupt gate
    // 1 00         0           1                110
    insert_idt_entry(0, (unsigned int)isr0, 0x08, 0x8E);
    insert_idt_entry(1, (unsigned int)isr1, 0x08, 0x8E);
    
    insert_idt_entry(2, (unsigned int)isr2, 0x08, 0x8E);
    insert_idt_entry(3, (unsigned int)isr3, 0x08, 0x8E);
    
    insert_idt_entry(4, (unsigned int)isr4, 0x08, 0x8E);
    insert_idt_entry(5, (unsigned int)isr5, 0x08, 0x8E);
    insert_idt_entry(6, (unsigned int)isr6, 0x08, 0x8E);
    insert_idt_entry(7, (unsigned int)isr7, 0x08, 0x8E);
    insert_idt_entry(8, (unsigned int)isr8, 0x08, 0x8E);
    insert_idt_entry(9, (unsigned int)isr9, 0x08, 0x8E);
    insert_idt_entry(10, (unsigned int)isr10, 0x08, 0x8E);
    insert_idt_entry(11, (unsigned int)isr11, 0x08, 0x8E);
    insert_idt_entry(12, (unsigned int)isr12, 0x08, 0x8E);
    insert_idt_entry(13, (unsigned int)isr13, 0x08, 0x8E);
    insert_idt_entry(14, (unsigned int)isr14, 0x08, 0x8E);
    insert_idt_entry(15, (unsigned int)isr15, 0x08, 0x8E);
    insert_idt_entry(16, (unsigned int)isr16, 0x08, 0x8E);
    insert_idt_entry(17, (unsigned int)isr17, 0x08, 0x8E);
    insert_idt_entry(18, (unsigned int)isr18, 0x08, 0x8E);
    insert_idt_entry(19, (unsigned int)isr19, 0x08, 0x8E);
    insert_idt_entry(20, (unsigned int)isr20, 0x08, 0x8E);
    insert_idt_entry(21, (unsigned int)isr21, 0x08, 0x8E);
    insert_idt_entry(22, (unsigned int)isr22, 0x08, 0x8E);
    insert_idt_entry(23, (unsigned int)isr23, 0x08, 0x8E);
    insert_idt_entry(24, (unsigned int)isr24, 0x08, 0x8E);
    insert_idt_entry(25, (unsigned int)isr25, 0x08, 0x8E);
    insert_idt_entry(26, (unsigned int)isr26, 0x08, 0x8E);
    insert_idt_entry(27, (unsigned int)isr27, 0x08, 0x8E);
    insert_idt_entry(28, (unsigned int)isr28, 0x08, 0x8E);
    insert_idt_entry(29, (unsigned int)isr29, 0x08, 0x8E);
    insert_idt_entry(30, (unsigned int)isr30, 0x08, 0x8E);
    insert_idt_entry(31, (unsigned int)isr31, 0x08, 0x8E);
    
}

void setup_irqs(void) {
    reprogram_pic();

    insert_idt_entry(32, (unsigned int)irq0, 0x08, 0x8E);
    insert_idt_entry(33, (unsigned int)irq1, 0x08, 0x8E);
    insert_idt_entry(34, (unsigned int)irq2, 0x08, 0x8E);
    insert_idt_entry(35, (unsigned int)irq3, 0x08, 0x8E);
    insert_idt_entry(36, (unsigned int)irq4, 0x08, 0x8E);
    insert_idt_entry(37, (unsigned int)irq5, 0x08, 0x8E);
    insert_idt_entry(38, (unsigned int)irq6, 0x08, 0x8E);
    insert_idt_entry(39, (unsigned int)irq7, 0x08, 0x8E);
    insert_idt_entry(40, (unsigned int)irq8, 0x08, 0x8E);
    insert_idt_entry(41, (unsigned int)irq9, 0x08, 0x8E);
    insert_idt_entry(42, (unsigned int)irq10, 0x08, 0x8E);
    insert_idt_entry(43, (unsigned int)irq11, 0x08, 0x8E);
    insert_idt_entry(44, (unsigned int)irq12, 0x08, 0x8E);
    insert_idt_entry(45, (unsigned int)irq13, 0x08, 0x8E);
    insert_idt_entry(46, (unsigned int)irq14, 0x08, 0x8E);
    insert_idt_entry(47, (unsigned int)irq15, 0x08, 0x8E);
}

void handle_exception(struct reg_states *regs) {
    unsigned int i_num = regs->interrupt_num;
    if(i_num < 32) {
        kprintf(exception_messages[i_num > 21 ? 21 : i_num]);
        kprintf("You fucked up\n");
        while(1);
    } 
}

void handle_page_fault(struct reg_states *regs) {
   unsigned int addr;
   asm volatile("mov %%cr2, %0" : "=r" (addr));
}

void handle_irq(struct reg_states *regs) {
    //STOP_INTERRUPTS;
    unsigned int i_num = regs->interrupt_num;
    void (*irq_handler)(struct reg_states *) = irq_routines[i_num - 32];
    if(irq_handler)
        irq_handler(regs);
    
    //interrupt num was for slave controller entry
    if(i_num >= 40) {
        write_port(SLAVE_COMMAND, END_OF_INTERRUPT);
        io_wait();
    }
    //send EOI to master
    write_port(MASTER_COMMAND, END_OF_INTERRUPT);
    io_wait();
    //RESUME_INTERRUPTS;
}
