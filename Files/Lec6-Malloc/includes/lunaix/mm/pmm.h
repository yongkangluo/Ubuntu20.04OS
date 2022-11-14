#ifndef __LUNAIX_PMM_H
#define __LUNAIX_PMM_H
// Physical memory manager

#include <stddef.h>
#include <stdint.h>

#define PM_PAGE_SIZE 4096
#define PM_BMP_MAX_SIZE (1024 * 1024)

typedef uint32_t pp_attr_t;

void
pmm_mark_page_free(uintptr_t ppn);

/**
 * @brief 标注物理页为已占用
 *
 * @param ppn
 */
void
pmm_mark_page_occupied(uintptr_t ppn);

/**
 * @brief 标注多个连续的物理页为可用
 *
 * @param start_ppn 起始PPN
 * @param page_count 数量
 */
void
pmm_mark_chunk_free(uintptr_t start_ppn, size_t page_count);



void
pmm_mark_chunk_occupied(uint32_t start_ppn, size_t page_count);

void
pmm_init(uintptr_t mem_upper_lim);


void*
pmm_alloc_page();


int
pmm_free_page(void* page);


#endif /* __LUNAIX_PMM_H */
