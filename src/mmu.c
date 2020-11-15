/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

#define MMU_FLAG_PRESENT 0x01
#define MMU_FLAG_SUPERVISOR 0x00
#define MMU_FLAG_READ_WRITE 0x01

void mmu_init(void) {}

paddr_t mmu_next_free_kernel_page(void) {
  return 0;
}

paddr_t mmu_init_kernel_dir(void) {
  page_directory_entry *pd = (page_directory_entry *)KERNEL_PAGE_DIR;
  page_table_entry *pt_0 = (page_table_entry *)KERNEL_PAGE_TABLE_0;

  for (int i = 0; i < 1024; i++)
  {
    pd[i] = (page_directory_entry){0};
    pt_0[i] = (page_table_entry){0};
  }

  pd[0].present = MMU_FLAG_PRESENT;
  pd[0].user_supervisor = MMU_FLAG_SUPERVISOR;
  pd[0].read_write = MMU_FLAG_READ_WRITE;
  pd[0].page_table_base = ((uint32_t)pt_0 >> 12); // 12 bits mas significativos de la tabla

  for (int i = 0; i < 1024; i++)
  {
    pt_0[i].present = MMU_FLAG_PRESENT;
    pt_0[i].user_supervisor = MMU_FLAG_SUPERVISOR;
    pt_0[i].read_write = MMU_FLAG_READ_WRITE;
    pt_0[i].physical_adress_base = i;
  }

  return (uint32_t)0;
}

// void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {}
// paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {}
// paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t
// pages) {}

