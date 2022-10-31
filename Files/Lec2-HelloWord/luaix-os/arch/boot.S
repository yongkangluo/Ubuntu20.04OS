#include "multiboot.h"
.section .multiboot
    .long MB_MAGIC
    .long MB_ALIGNED_4K_MEM_MAP
    .long CHECKSUM(MB_ALIGNED_4K_MEM_MAP)

.section .bss
    .align 16
    stack_bottm:
        .skip 16318, 0
    stack_top:

.section .text
    .global start_
    start_:
        movl $stack_top, %esp
        
        call _kernel_init

        pushl %ebx
        call _kernel_main

        cli 
    j_:
        hlt
        jmp j_
