.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

#Divide by Zero Exception
isr0:
    cli
    push $0
    push $0
    jmp save_state
isr1:
    cli
    push $0
    push $1
    jmp save_state
isr2:
    cli
    push $0
    push $2
    jmp save_state
isr3:
    cli
    push $0
    push $3
    jmp save_state
isr4:
    cli
    push $0
    push $4
    jmp save_state
isr5:
    cli
    push $0
    push $5
    jmp save_state
isr6:
    cli
    push $0
    push $6
    jmp save_state
isr7:
    cli
    push $0
    push $7
    jmp save_state

#Double Fault Exception
isr8:
    cli
    push $0
    jmp save_state
isr9:
    cli
    push $0
    push $9
    jmp save_state
isr10:
    cli
    push $0
    push $10
    jmp save_state
isr11:
    cli
    push $0
    push $11
    jmp save_state
isr12:
    cli
    push $0
    push $12
    jmp save_state
isr13:
    cli
    push $0
    push $13
    jmp save_state
isr14:
    cli
    push $0
    push $14
    jmp save_state
isr15:
    cli
    push $0
    push $15
    jmp save_state
isr16:
    cli
    push $0
    push $16
    jmp save_state
isr17:
    cli
    push $0
    push $17
    jmp save_state
isr18:
    cli
    push $0
    push $18
    jmp save_state
isr19:
    cli
    push $0
    push $19
    jmp save_state
isr20:
    cli
    push $0
    push $20
    jmp save_state
isr21:
    cli
    push $0
    push $21
    jmp save_state
isr22:
    cli
    push $0
    push $22
    jmp save_state
isr23:
    cli
    push $0
    push $23
    jmp save_state
isr24:
    cli
    push $0
    push $24
    jmp save_state
isr25:
    cli
    push $0
    push $25
    jmp save_state
isr26:
    cli
    push $0
    push $26 
    jmp save_state
isr27:
    cli
    push $0
    push $27
    jmp save_state
isr28:
    cli
    push $0
    push $28
    jmp save_state
isr29:
    cli
    push $0
    push $29
    jmp save_state
isr30:
    cli
    push $0
    push $30
    jmp save_state
isr31:
    cli
    push $0
    push $31
    jmp save_state

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
