.section .text
.align 4

.global load_idt
.extern idt_ptr

load_idt:
    //movl 4(%esp), %eax
    lidt idt_ptr
    ret
    //sidtr ($ptr), 0x00AB
