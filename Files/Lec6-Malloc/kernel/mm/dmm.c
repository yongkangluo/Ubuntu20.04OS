#include <lunaix/mm/dmm.h>
#include <lunaix/mm/vmm.h>
#include <lunaix/mm/page.h>
#include <lunaix/spike.h>


int
dmm_init(heap_context_t* heap){
    // assert((uintptr_t) heap -> start % BOUNDARY == 0);
    // 开始字节是不是对齐的
    heap -> brk = heap-> start;
    return  vmm_alloc_page(heap->brk, PG_PREM_RW) != NULL;
}


int
lxsbrk(heap_context_t* heap, void* addr){
    return lxbrk(heap, addr - heap->brk) != NULL;
}
 
void*
lxbrk(heap_context_t * heap, size_t size){
    if(size == 0){
        return heap->brk;
    }
    void* current_brk = heap->brk;
    void* next = current_brk + ROUNDUP(size, BOUNDARY);
    if(next >= heap->max_addr || next < current_brk){
        //防止超过上界和重写
        return NULL;
    }    
    uintptr_t diff = PG_ALIGN(next) - PG_ALIGN(current_brk);
    if(diff){
        if(!vmm_alloc_pages((void*)(PG_ALIGN(current_brk) + PG_SIZE), diff, PG_PREM_RW)){
            // assert_msg(0, "unable to brk");
            return NULL;
        }
    }
    heap->brk += size;
    return current_brk;
}
