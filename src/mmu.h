/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "types.h"

#include "defines.h"
// #include "tss.h"
// #include "game.h"
typedef struct str_page_directory_entry{
  uint8_t present : 1;
  uint8_t read_write : 1;
  uint8_t user_supervisor : 1;
  uint8_t page_write_through : 1;
  uint8_t page_cache_disable : 1;
  uint8_t accessed : 1;
  uint8_t x : 1;
  uint8_t page_size : 1;
  uint8_t ignored : 1;
  uint8_t available : 3;
  uint32_t page_table_base :20;
} __attribute__ ((packed)) page_directory_entry;

typedef struct str_page_table_entry
{
  uint8_t present : 1;
  uint8_t read_write : 1;
  uint8_t user_supervisor : 1;
  uint8_t page_write_through : 1;
  uint8_t page_cache_disable : 1;
  uint8_t accessed : 1;
  uint8_t dirty : 1;
  uint8_t x : 1;
  uint8_t global : 1;
  uint8_t available : 3;
  uint32_t physical_adress_base : 20;
} __attribute__((packed)) page_table_entry;

void mmu_init(void);

paddr_t mmu_next_free_kernel_page(void);

void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs);

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt);

paddr_t mmu_init_kernel_dir(void);

void mmu_map_kernel(paddr_t cr3);
void mmu_unmap_kernel(paddr_t cr3);

paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t pages);

void mmu_init_task_meeseeks_dir(paddr_t phy_start, paddr_t code_start, paddr_t tasks_virt_memory);



paddr_t mmu_next_free_virt_meeseek_page();
paddr_t mmu_next_free_phy_meeseek_page();

typedef struct info_meeseek
{
  // paddr_t phy;
  bool p;
  paddr_t virt;
  paddr_t stack_level_0;
  paddr_t cr3;
} backup_meesek;

extern backup_meesek backup_meeseks[PLAYERS][MAX_CANT_MEESEEKS];

#endif //  __MMU_H__
