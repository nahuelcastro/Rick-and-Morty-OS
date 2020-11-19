/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
#include "types.h"

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


  //attrs = attrs;

  int off_pd = (virt >> 22);
  int off_pt = ((virt << 10) >> 22);
  
  
  //uint8_t FLAG_PRESENT = (uint8_t)((attrs << 31) >> 31);
  uint8_t FLAG_USER_SUPERVISOR = (uint8_t)((attrs << 30) >> 31);    // ver si va siempre en 0
  uint8_t FLAG_READ_WRITE = (uint8_t)((attrs << 29) >> 31);


 /*
  uint8_t FLAG_PAGE_WRITE_THROUGH =((attrs << 28) >> 31);
  uint8_t FLAG_PAGE_CACHE_DISABLE =((attrs << 27) >> 31);
  uint8_t FLAG_ACCESSED = ((attrs << 26) >> 31);
  uint8_t FLAG_X = ((attrs << 25) >> 31);
  uint8_t FLAG_PAGE_SIZE = ((attrs << 24) >> 31);
  uint8_t FLAG_IGNORED = ((attrs << 23) >> 31);
  uint8_t FLAG_AVAILABLE = ((attrs << 20) >> 29);
  */
    
  page_directory_entry* pde_map = (page_directory_entry*) cr3;

  page_table_entry* pte_map =(page_table_entry*) 0;

  if(pde_map[off_pd].present==0){
    
    paddr_t free_page = mmu_next_free_kernel_page();

    for (int i = 0; i < 1024; i++){
      pte_map[i] = (page_table_entry){0};
  }

    pde_map[off_pd].present = 1;
    pde_map[off_pd].read_write = FLAG_READ_WRITE;
    pde_map[off_pd].user_supervisor = FLAG_USER_SUPERVISOR;
    pde_map[off_pd].page_write_through = 0;
    pde_map[off_pd].page_cache_disable = 0;
    pde_map[off_pd].accessed = 0;
    pde_map[off_pd].x = 0;
    pde_map[off_pd].page_size = 0; 
    pde_map[off_pd].ignored = 0;
    pde_map[off_pd].available = 0;
    pde_map[off_pd].page_table_base = ((uint32_t)free_page >> 12); 
    

    pte_map = (page_table_entry*) free_page;
  
  }else{

    pte_map = (page_table_entry*) (pde_map[off_pd].page_table_base << 12);
  
  }
  
  if(pte_map[off_pt].present == 0){  // si presente esta en 1 ya esta mapeada 
    
    pte_map[off_pt].present = 1;
    pte_map[off_pt].read_write = FLAG_READ_WRITE;
    pte_map[off_pt].user_supervisor = FLAG_USER_SUPERVISOR;
    pte_map[off_pt].page_write_through = 0;
    pte_map[off_pt].page_cache_disable = 0;
    pte_map[off_pt].accessed = 0;
    pte_map[off_pt].dirty = 0;
    pte_map[off_pt].x = 0;
    pte_map[off_pt].global = 0;
    pte_map[off_pt].available = 0; 
    pte_map[off_pt].physical_adress_base = phy >>12;
  }    

  tlbflush();

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

paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t pages) {


  uint32_t new_cr3 = mmu_next_free_kernel_page();
  
  uint32_t cr3 = rcr3();

  // no quiero perder las direcciones iniciales
  paddr_t original_phy_page = phy_start;
  
  uint8_t* ptr_code_page = (uint8_t*) code_start;
  uint8_t* ptr_phy_page = (uint8_t*) phy_start;
  
  paddr_t virt_page = 0x1D00000;

  // Mapeando en el page directory del kernel
  for (size_t i = 0; i < pages; i++){
    mmu_map_page(cr3,virt_page, phy_start, 2);
    virt_page += 4096; // 4096 = 4kb = tamaño pagina
    phy_start += 4096;  // 4096 = 4kb = tamaño pagina
  }
  
   
  virt_page = 0x1D00000;
  phy_start = original_phy_page;

  //Mapeando en el page directory nuevo
  for (size_t i = 0; i < pages; i++){
    mmu_map_page(new_cr3,virt_page, phy_start, 2);
    virt_page += 4096; // 4096 = 4kb = tamaño pagina
    phy_start += 4096;  // 4096 = 4kb = tamaño pagina
  }

  // copiar las tareas de rick o morty del kernel a codigo respectivo 
  for (size_t i = 0; i < 16384; i++){
    *ptr_phy_page = *ptr_code_page;
    ptr_code_page++;
    ptr_phy_page++;
  }

  virt_page = 0x1D00000;
  
  // Desmapeando de cr3 de Kernel para que después no me rompa lo de Rick
  for (size_t i = 0; i < pages; i++){
    mmu_unmap_page(cr3, virt_page);
    virt_page += 4096; // 4096 = 4kb = tamaño pagina(en bytes)cal
  }

  return new_cr3;

}

paddr_t mmu_prueba(paddr_t cr3, paddr_t im, int attrs){
  
  paddr_t phy = im;
  paddr_t virt = im;

  for (size_t i = 0; i < 10; i++){
    mmu_map_page(cr3,virt,phy,attrs);
    virt += 4096;
    phy += 4096;
  }

  return cr3;
  
}



  // for (size_t i = 0; i < 512; i++){
  //   *ptr_phy_page = *ptr_code_page;
  //   ptr_phy_page++;
  //   ptr_code_page++;
  // }

// Crear una función que retorne la dirección física de un page directory que tenga mapeado con identity mapping el kernel, 
// y el código de rick/morty en las direcciones que dice el enunciado, como indica la figura creo que 5 o 3.
//void 


