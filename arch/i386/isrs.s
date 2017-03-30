.section .text

.macro NO_ERR_ISR CODE
    .global isr\CODE
    isr\CODE:
    cli
    push $0
    push $\CODE
    jmp save_state
.endm

.macro ERR_ISR CODE
    .global isr\CODE
    isr\CODE:
    cli
    push $\CODE
    jmp save_state
.endm

NO_ERR_ISR 0
NO_ERR_ISR 1
NO_ERR_ISR 2
NO_ERR_ISR 3
NO_ERR_ISR 4
NO_ERR_ISR 5
NO_ERR_ISR 6
NO_ERR_ISR 7
ERR_ISR 8
NO_ERR_ISR 9
ERR_ISR 10
ERR_ISR 11
ERR_ISR 12
ERR_ISR 13
ERR_ISR 14
NO_ERR_ISR 15 #intel reserved
NO_ERR_ISR 16
ERR_ISR 17
NO_ERR_ISR 18
NO_ERR_ISR 19
NO_ERR_ISR 20

#intel reserved
NO_ERR_ISR 21

NO_ERR_ISR 22

NO_ERR_ISR 23

NO_ERR_ISR 24

NO_ERR_ISR 25

NO_ERR_ISR 26

NO_ERR_ISR 27

NO_ERR_ISR 28

NO_ERR_ISR 29

NO_ERR_ISR 30

NO_ERR_ISR 31





.extern handle_exception

save_state:
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
    call handle_exception

    pop %esp
    pop %gs
    pop %fs
    pop %es
    pop %ds

    popa
    addl $8, %esp
    iret
