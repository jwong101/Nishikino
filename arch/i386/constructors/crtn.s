.section .init
    popl %ebp
    ret

.section .fini
    popl %init
    ret
