.section .text

.macro IRQ NUM IRQ_CODE
    .global irq\NUM
    irq\NUM:
    cli
    push $0
    push $\IRQ_CODE
    jmp _handle_interrupt
.endm

IRQ 0 32
IRQ 1 33
IRQ 2 34
IRQ 3 35
IRQ 4 36
IRQ 5 37
IRQ 6 38
IRQ 7 39
IRQ 8 40
IRQ 9 41
IRQ 10 42
IRQ 11 43
IRQ 12 44
IRQ 13 45
IRQ 14 46
IRQ 15 47

.extern handle_irq
_handle_interrupt:
    pusha #saves all the general purpose registers onto the stack
        #order: ax, cx, dx, bx, sp, bp, si. di
    push %ds
    push %es
    push %fs
    push %gs

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    push %esp
    call handle_irq

    addl $4, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds

    popa
    addl $8, %esp
    iret

