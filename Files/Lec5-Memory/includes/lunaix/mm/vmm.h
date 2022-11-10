#ifndef __LUNAIX_VMM_H
#define __LUNAIX_VMM_H
#include <lunaix/mm/page.h>
#include <stddef.h>
#include <stdint.h>
// Virtual memory manager

/**
 * @brief 初始化虚拟内存管理器
 *
 */
void
vmm_init();

/**
 * @brief 创建一个页目录
 *
 * @return ptd_entry* 页目录的物理地址，随时可以加载进CR3
 */
ptd_t*
vmm_init_pd();

/**
 * @brief 在指定地址空间中，添加一个映射
 * @param vpa 虚拟地址
 * @param ppa 物理地址
 * @param dattr 映射属性
 * @param tattr 映射属性
 * @return int
 */
void*
vmm_map_page(void* vpn, void * ppn, pt_attr dattr, pt_attr tattr);

void*
vmm_alloc_page(void* vpn, pt_attr dattr, pt_attr tattr);

/**
 * @brief 删除一个映射
 * @param vpn
 */
void vmm_unmap_page(void * vpn);


ptd_t* get_pd();

void set_pg(ptd_t* pd);

void* vmm_v2p(void* va);

#endif /* __LUNAIX_VMM_H */
