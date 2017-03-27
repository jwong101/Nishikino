.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

#grub needs this multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

#set up stack
.section .bss
.align 16
stack_bottom:
.skip 32768 #32 kilobytes

stack_top:

.section .text
.global _start
.extern kernel_main

.type _start, @function
_start:
    pushl %esp #save sp (for later)
    pushl %ebp
    cli
    call kernel_main

    cli
#hang the cpu if kernel_main returns 
hang:
    hlt
    jmp hang #for insurance
