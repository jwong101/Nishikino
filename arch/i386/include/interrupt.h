#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define STOP_INTERRUPTS asm("cli")
#define RESUME_INTERRUPTS asm("sti")

#define INIT_PIC 0x10
#define ICW4  0x01

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xA0

#define MASTER_COMMAND 0x20
#define MASTER_DATA 0x21

#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA 0xA1

#define END_OF_INTERRUPT 0x20

#define PIC_8086_MODE 0x01

#define MASTER_OFFSET 0x20 //32
#define SLAVE_OFFSET 0x28 //40

struct reg_states {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int interrupt_num, error_code;
    unsigned int eip, cs, eflags, sp, ss;
} __attribute__((packed));


void init_idt(void);
void setup_interrupt_service_routines(void);
void setup_irqs(void);
void reprogram_pic(void);
void handle_irq(struct reg_states *regs);
void handle_exception(struct reg_states *regs);
void install_irq_entry(unsigned char index, void (*irq_routine)(struct reg_states *regs));
void setup_irq_routines(void);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
