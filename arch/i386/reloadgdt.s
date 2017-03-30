.section .text
.align 4

.global gdt_flush
.extern ptr

gdt_flush:
    movl 4(%esp), %eax
    
    lgdt (%eax)

    movw $0x10, %ax #0x10 is the new selector (third entry in gdt)
    
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    ljmp $0x08, $flush #0x08 is the new selector (second entry in gdt)
    
flush:
    ret
