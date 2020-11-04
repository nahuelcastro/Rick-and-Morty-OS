/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

void mmu_init(void) {}

paddr_t mmu_next_free_kernel_page(void) {
  return 0;
}

paddr_t mmu_init_kernel_dir(void) {
  return 0;
}

// void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {}
// paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {}
// paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t
// pages) {}

