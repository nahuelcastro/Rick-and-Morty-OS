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

uint32_t next_free_kernel_page;
uint32_t next_free_task_page;

void mmu_init(void) {
  //En memoria fisica, 0x100000 es el inicio del área libre de kernel.
  next_free_kernel_page = 0x100000;

  //En memoria fisica, 0x400000 es el inicio del área libre de tareas.
  next_free_task_page = 0x400000;
}

paddr_t mmu_next_free_kernel_page(void) {
  paddr_t free_page = next_free_kernel_page;
  next_free_kernel_page += 0x1000;
  return free_page;
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

void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs){
  //virt | 0000 0000 01 (off_pd) | 01 0000 1110 (off_pt) | 0000 0010 0111 (off_dir) 

  attrs = attrs;

  int off_pd = (virt >> 22);
  int off_pt = ((virt << 10) >> 22);
  //int off_dir = ((virt << 20) >> 20);

  page_directory_entry* pde_map = (page_directory_entry*) cr3;

  page_table_entry* pte_map =(page_table_entry*) 0;

  if(pde_map[off_pd].present==0){
    
    paddr_t free_page = mmu_next_free_kernel_page();

    for (int i = 0; i < 1024; i++){
      pte_map[i] = (page_table_entry){0};
    }
    
    pde_map[off_pd].present = MMU_FLAG_PRESENT;
    pde_map[off_pd].user_supervisor = MMU_FLAG_SUPERVISOR;
    pde_map[off_pd].read_write = MMU_FLAG_READ_WRITE;
    pde_map[off_pd].page_table_base = ((uint32_t)free_page >> 12); 
    
    pte_map = (page_table_entry*) free_page;
  }else{
    pte_map = (page_table_entry*) (pde_map[off_pd].page_table_base << 12);
  }
  if(pte_map[off_pt].present == 0){  // si presente esta en 1 ya esta mapeada 
    pte_map[off_pt].present = MMU_FLAG_PRESENT;
    pte_map[off_pt].user_supervisor = MMU_FLAG_SUPERVISOR;
    pte_map[off_pt].read_write = MMU_FLAG_READ_WRITE;  
    uint32_t phyBase = (phy /*- off_dir*/) >> 12;
    pte_map[off_pt].physical_adress_base = phyBase;
  }    
}

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt){
  int off_pd = (virt >> 22);
  int off_pt = ((virt << 10) >> 22);
  //int off_dir = ((virt << 20) >> 20);

  page_directory_entry* pde_map = (page_directory_entry*) cr3;
  
  page_table_entry* pte_map = (page_table_entry*) (pde_map[off_pd].page_table_base << 12);
  
  pte_map[off_pt].present = 0; //Al tener presente en 0 lo va a desmapear 
  paddr_t phyAddr = (pte_map[off_pt].physical_adress_base >> 12); //off_dir;
  return phyAddr;

}


// void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {}
// paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {}
// paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t
// pages) {}

