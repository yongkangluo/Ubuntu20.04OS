#include <stdint.h>

#include <lunaix/common.h>
#include <lunaix/tty/tty.h>

#include <lunaix/mm/page.h>
#include <lunaix/mm/pmm.h>
#include <lunaix/mm/vmm.h>

#include <hal/cpu.h>

#include <arch/x86/boot/multiboot.h>
#include <arch/x86/idt.h>
#include <arch/x86/gdt.h>

#include <libc/stdio.h>

extern void __kernel_start;
extern void __kernel_end; 
extern void __init_hhk_end;

void
_kernel_init(multiboot_info_t* mb_info)
{
    _init_idt();

    multiboot_memory_map_t* map = (multiboot_memory_map_t*)mb_info->mmap_addr;








#pragma region INIT_MM
    pmm_init(MEM_1MB + mb_info->mem_upper << 10);
    vmm_init();
#pragma endregion


//初始化VGA
    tty_init(VGA_BUFFER_PADDR);
    tty_set_theme(VGA_COLOR_GREEN, VGA_COLOR_BLACK);

    printf("[KERNEL]*****Initialization*****\n");
    unsigned int map_size =
        mb_info->mmap_length / sizeof(multiboot_memory_map_t);
    printf( "[MM] Mem: %d kiB, Extended Mem: %d kiB\n",
            mb_info->mem_lower,
            mb_info->mem_upper);

#pragma region MMAP_SCAN_RESERVING_KERNEL_PGS

    for (unsigned int i = 0; i < map_size; i++) {
        multiboot_memory_map_t mmap = map[i] ;
        printf("[MM] Base: 0x%x, Len: %u KiB, type: %u\n",
                map[i].addr_low, 
                map[i].len_low >> 10,
                map[i].type);
        if (mmap.type == MULTIBOOT_MEMORY_AVAILABLE) {
            //整数向上取整除法
            uintptr_t pg = map[i].addr_low + 0x0fffU;
            pmm_mark_chunk_free( pg >> 12, map[i].len_low >> 12);
            printf(" [MM] Freed %u pages start from 0x%x\n",
                map[i].len_low >> 12,
                pg & ~0x0fffU);
        }   
    }


    size_t pg_count = (uintptr_t)(&__kernel_end - &__kernel_start) >> 12;
    pmm_mark_chunk_occupied(V2P(&__kernel_start) >> 12, pg_count);
    printf(" [MM] Allocated %d pages for kernel. \n", pg_count);
#pragma endregion

    size_t vga_buf_pgs = VGA_BUFFER_SIZE >> 12;

    pmm_mark_chunk_occupied(VGA_BUFFER_PADDR >> 12, vga_buf_pgs);

    for(size_t i = 0; i < vga_buf_pgs; i++){ 
        vmm_map_page(VGA_BUFFER_VADDR + (i << 12), VGA_BUFFER_PADDR + (i << 12), PG_PREM_RW, PG_PREM_RW);
    }

    tty_set_buffer(VGA_BUFFER_VADDR);

    printf("[MM] Mapped VGA to %p.\n", VGA_BUFFER_VADDR);

    for(size_t i=0; i < (KSTACK_SIZE >> 12); i++){
        vmm_alloc_page(KSTACK_START + (i << 12), PG_PREM_RW, PG_PREM_RW);
    }
    printf("[MM] AlLocated %d pages for stack start at %p\n", KSTACK_SIZE >> 12, KSTACK_START);
    
    printf(" [KERNEL] = Initialization Done = \n\n");

}

void 
_kernel_post_init(){

    printf(" [KERNEL] = Post Initialization = \n");
    size_t hhk_init_pg_count = ((uintptr_t)(&__init_hhk_end)) >> 12;
    printf(" [MM] Releaseing %d pages from 0x0. \n", hhk_init_pg_count);

//清除hhk_ init 与前1MiB的映射
    for (size_t i = 0; i < hhk_init_pg_count; i++) {
        vmm_unmap_page((i << 12));
    }
    printf(" [KERNEL] = Post Initialization Done = \n\n");

}

void _kernel_main(){
    char buf[64];
    printf("Hello higher Kernel! \n");
    cpu_get_brand(buf);
    uintptr_t k_start = vmm_v2p(&__kernel_start);

    printf("THe kernel based address mapping : %p -> %p \n", &__kernel_start, k_start);
}