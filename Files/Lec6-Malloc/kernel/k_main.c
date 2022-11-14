#include <stdint.h>

#include <lunaix/common.h>
#include <lunaix/tty/tty.h>

#include <lunaix/mm/page.h>
#include <lunaix/mm/pmm.h>
#include <lunaix/mm/vmm.h>
#include <lunaix/mm/kalloc.h>

#include <hal/cpu.h>

#include <arch/x86/boot/multiboot.h>
#include <arch/x86/idt.h>
#include <arch/x86/gdt.h>

#include <libc/stdio.h>

extern void __kernel_start;
extern void __kernel_end; 
extern void __init_hhk_end;

void _kernel_main(){
    kalloc_init();
    char buf[64];
    printf("Hello higher Kernel! \n");
    cpu_get_brand(buf);
    uintptr_t k_start = vmm_v2p(&__kernel_start);
    printf("THe kernel based address mapping : %p -> %p \n", &__kernel_start, k_start);
    
    uint8_t** arr = (uint8_t**) lxmalloc(10 * sizeof(uint8_t*));
    for (size_t i = 0; i < 10; i++)
    {
        arr[i] = (uint8_t*) lxmalloc((i + 1) * 2);
    }
    for(size_t i = 0; i < 10; ++i){
        lxfree(arr[i]);
    }
    uint8_t* big_ = lxmalloc(8296);
    big_[0] = 123;
    big_[1] = 13;
    big_[2] = 23;
    printf("%u, %u, %u", big_[0], big_[1], big_[2]);
    lxfree(arr);
    lxfree(big_);
}