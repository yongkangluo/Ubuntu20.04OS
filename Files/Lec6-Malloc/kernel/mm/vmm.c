#include <libc/string.h>
#include <lunaix/mm/pmm.h>
#include <lunaix/mm/vmm.h>
#include <lunaix/mm/page.h>
#include <hal/cpu.h>

ptd_t* get_pd() {
    ptd_t* pd;
    #ifdef __ARCH_IA32
    __asm__(
        "movl %%cr3, %0\n"
        "andl $0xfffff000, %0"
        : "=r"(pd)
    );
    #endif
    return P2V(pd);
}

void set_pd(ptd_t* pd) {
    #ifdef __ARCH_IA32
    __asm__(
        "movl %0, %%eax\n"
        "andl $0xfffff000, %%eax\n "
        "movl %%eax, %%cr3\n"
        :
        : "r" (pd)
    );
    #endif
}


void vmm_init()
{
    // XXX: something here?
}

ptd_t*
vmm_init_pd()
{
    ptd_t* dir = pmm_alloc_page();
    for (size_t i = 0; i < 1024; i++) {
        dir[i] = 0;
    }

    // 递归映射，方便我们在软件层面进行查表地址转换
    dir[1023] = NEW_L1_ENTRY(T_SELF_REF_PERM, dir);

    return dir;
}

void*
vmm_map_page(void* va,
                void* pa,
                pt_attr dattr,
                pt_attr tattr){
    if(!pa || !va){
        return NULL;
    }
    uintptr_t pd_offset = L1_INDEX(va);
    uintptr_t pt_offset = L2_INDEX(va);
    ptd_t* ptd = (ptd_t*)L1_BASE_VADDR;


    ptd_t* pde = ptd[pd_offset];
    pt_t* pt = (uintptr_t)L2_VADDR(pd_offset);

    while (pde && pd_offset < 1024) {
        if (pt_offset == 1024) {
            pd_offset++ ;
            pt_offset = 0;
            pde = ptd[pd_offset];
            pt = (pt_t *)L2_VADDR(pd_offset);
        }
        //页表有空位,只需要开辟一个新的PTE
        if (pt && !pt[pt_offset]) {
            pt[pt_offset] = NEW_L2_ENTRY(tattr, pa);
            return V_ADDR(pd_offset, pt_offset, PG_OFFSET(va));
        }
        pt_offset++ ;
        //页目录与所有页表已满!
    }
    if (pd_offset > 1024) {
        return NULL ;
    }
        //页目录有空位,需要开辟一个新的PDE
    uint8_t* new_pt_pa = pmm_alloc_page();
    //物理内存已满!
    if (!new_pt_pa) {
        return NULL;
    }

    ptd[pd_offset] = NEW_L1_ENTRY(dattr, new_pt_pa);

    memset((void *)L2_VADDR(pd_offset), 0, PM_PAGE_SIZE);
    
    pt[pt_offset] = NEW_L2_ENTRY(tattr, pa);

    return V_ADDR(pd_offset, pt_offset, PG_OFFSET(va));
}

void* vmm_alloc_page(void* vpn, pt_attr dattr) {
    void* pp = pmm_alloc_page() ;
    void* result = vmm_map_page(vpn, pp, dattr, dattr);
    if (!result) {
        pmm_free_page(pp);
    }
    return result;
}

void* vmm_alloc_pages(void* vpn, uintptr_t diff, pt_attr tattr) {
    size_t i = 0;
    while(i < diff){
        void* pp = pmm_alloc_page() ;
        void* result = vmm_map_page(vpn, pp + i, tattr, tattr);
        if (!result) {
            pmm_free_page(pp);
            return result;
        }
        i += 4096;
    }
}


void vmm_unmap_page(void* vpn) {
    uintptr_t pd_offset = L1_INDEX(vpn);
    uintptr_t pt_offset = L2_INDEX(vpn);
    ptd_t* self_pde = L1_BASE_VADDR;
    ptd_t pde = self_pde[pd_offset];
    if (pde) {
        pt_t* pt = (pt_t*)L2_VADDR(pd_offset);
        uint32_t pte = pt[pt_offset];
        if (IS_CACHED(pte) && pmm_free_page(pte)) {
            //刷新TLB
            #ifdef __ARCH_IA32
            __asm__ ("invlpg (%0)" :: "r"((uintptr_t)vpn) : "memory");
            #endif
        }
        pt[pt_offset] = 0;
    }
}


void* vmm_v2p(void* va) {
    uintptr_t pd_offset = L1_INDEX(va);
    uintptr_t pt_offset = L2_INDEX(va);
    uintptr_t po = PG_OFFSET(va);
    ptd_t* self_pde = L1_BASE_VADDR;
    ptd_t pde = self_pde[pd_offset];
    if (pde) {
        pt_t pte = ((pt_t*)L2_VADDR(pd_offset))[pt_offset];
        if (pte) {
            uintptr_t ppn = pte >> 12;
            return (void*)P_ADDR(ppn, po);
        }
    }
    return NULL;
}
