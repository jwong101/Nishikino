.section .text

.global enable_paging
enable_paging:
    pushl %ebp
    movl %esp, %ebp
    movl %cr0, %eax
    or $0x80000000, %eax
    movl %eax, %cr0
    movl %ebp, %esp
    popl %ebp
    ret

.global set_page_directory
set_page_directory:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %eax
    movl %eax, %cr3
    movl %ebp, %esp
    popl %ebp
    ret
