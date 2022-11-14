#include <stddef.h>

#define BOUNDARY 4
#define WSIZE 4

#define HEAP_INIT_SIZE 4096

#define PACK(size, flags) (((size) & ~0x3) | (flags))

#define SW(p, w) (*((uint32_t*)(p)) = w)
#define LW(p) (*((uint32_t*)(p)))



#define M_ALLOCATED 0x1
#define M_PREV_FREE 0x2

#define M_NOT_ALLOCATED 0x0
#define M_PREV_ALLOCATED 0x0


#define CHUNK_S(header) ((header) & ~0x3)
#define CHUNK_PF(header) ((header)&M_PREV_FREE)
#define CHUNK_A(header) ((header)&M_ALLOCATED)


#define HPTR(bp) ((uint32_t*)(bp)-1)
#define BPTR(bp) ((uint8_t*)(bp) + WSIZE)
#define FPTR(hp, size) ((uint32_t*)(hp + size - WSIZE))
#define NEXT_CHK(hp) ((uint8_t*)(hp) + CHUNK_S(LW(hp)))


typedef struct
{
    // 开始地址
    void* start;
    // 结束地址
    void* brk;
    // 最大地址
    void* max_addr;
} heap_context_t;

int
dmm_init(heap_context_t* heap);

int
lxsbrk(heap_context_t* heap, void* addr);

void*
lxbrk(heap_context_t* heap, size_t size); 

void*
lx_malloc_internal(heap_context_t* heap, size_t size);

void
lx_free_internal(void* ptr);
