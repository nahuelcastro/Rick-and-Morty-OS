/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"

#include "i386.h"
#include "kassert.h"
#include "types.h"

#define MMU_FLAG_PRESENT 0x01
#define MMU_FLAG_SUPERVISOR 0x00
#define MMU_FLAG_READ_WRITE 0x01

uint32_t next_free_kernel_page;
uint32_t next_free_virt_meeseek_page_rick;
uint32_t next_free_virt_meeseek_page_morty;


backup_meesek backup_meeseeks[PLAYERS][MAX_CANT_MEESEEKS];

void mmu_init(void) {
    // En memoria fisica, 0x100000 es el inicio del área libre de kernel.
    next_free_kernel_page = 0x100000;
    next_free_virt_meeseek_page_rick = 0x08000000;
    next_free_virt_meeseek_page_morty = 0x08000000;


    // seteo todos los reciclados como no presentes
    for (int player = 0; player < PLAYERS; player++) {
        for (int i = 0; i < MAX_CANT_MEESEEKS; i++) {
            backup_meeseeks[player][i].p = false;
        }
    }
}

paddr_t mmu_next_free_kernel_page(void) {
    paddr_t free_page = next_free_kernel_page;
    next_free_kernel_page += 0x1000;

    if(free_page > 0x3FFFFF){
        print("TE PASASTE DEL LIMITE EN EL KERNEL :(", 5, 5, RED_GREEN);
        breakpoint();
    }
    return free_page;
}

paddr_t mmu_next_free_virt_meeseek_page(player_t player) {
  if (player){
    paddr_t free_page = next_free_virt_meeseek_page_rick;
    next_free_virt_meeseek_page_rick += 0x2000;
    return free_page;
  }
  paddr_t free_page = next_free_virt_meeseek_page_morty;
  next_free_virt_meeseek_page_morty += 0x2000;
   return free_page;
}

paddr_t mmu_phy_map_decoder(coordenadas coord) {    
    paddr_t phy = PHY_INIT_MAP;
    phy = phy + (coord.x  * 2 * 4096) + (coord.y * 80 * 2 *4096);
    return phy;
}


paddr_t mmu_init_kernel_dir(void) {
    page_directory_entry *pd = (page_directory_entry *)KERNEL_PAGE_DIR;
    page_table_entry *pt_0 = (page_table_entry *)KERNEL_PAGE_TABLE_0;

    for (int i = 0; i < 1024; i++) {
        pd[i] = (page_directory_entry){0};
        pt_0[i] = (page_table_entry){0};
    }

    pd[0].present = MMU_FLAG_PRESENT;
    pd[0].user_supervisor = MMU_FLAG_SUPERVISOR;
    pd[0].read_write = MMU_FLAG_READ_WRITE;
    pd[0].page_table_base = ((uint32_t)pt_0 >> 12);  // 12 bits mas significativos de la tabla

    for (int i = 0; i < 1024; i++) {
        pt_0[i].present = MMU_FLAG_PRESENT;
        pt_0[i].user_supervisor = MMU_FLAG_SUPERVISOR;
        pt_0[i].read_write = MMU_FLAG_READ_WRITE;
        pt_0[i].physical_adress_base = i;
    }

    return (uint32_t)0;
}


void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
// primeros 10 bits off pd, segundos 10 off pt, ultimos 12 off page
    int off_pd = (virt >> 22);
    int off_pt = ((virt << 10) >> 22);

    uint8_t FLAG_USER_SUPERVISOR = (uint8_t)((attrs << 30) >> 31); 
    uint8_t FLAG_READ_WRITE = (uint8_t)((attrs << 29) >> 31);

    page_directory_entry *pde_map = (page_directory_entry *)cr3;

    page_table_entry *pte_map = (page_table_entry *)0;

    if (pde_map[off_pd].present == 0) {
        paddr_t free_page = mmu_next_free_kernel_page();
        pte_map = (page_table_entry *)free_page;

        for (int i = 0; i < 1024; i++) {
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
    } else {
        pte_map = (page_table_entry *)(pde_map[off_pd].page_table_base << 12);
    }

    if (pte_map[off_pt].present == 0) {  // si presente esta en 1 ya esta mapeada

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
        pte_map[off_pt].physical_adress_base = phy >> 12;
    }
    tlbflush();
}

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) { 

    int off_pd = (virt >> 22);
    int off_pt = ((virt << 10) >> 22);

    page_directory_entry *pde_map = (page_directory_entry *)cr3;

    page_table_entry *pte_map = (page_table_entry *)(pde_map[off_pd].page_table_base << 12);

    pte_map[off_pt].present = 0;  // Al tener presente en 0 lo va a desmapear
    paddr_t phyAddr = (pte_map[off_pt].physical_adress_base >> 12);  // off_dir;

    tlbflush();
    return phyAddr;
}

paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t pages){
    uint32_t new_cr3 = mmu_next_free_kernel_page();  // cr3: 0x000000105000
    uint32_t cr3 = 0x25000;

    paddr_t tasks_memory = TASK_CODE_VIRTUAL;

    mmu_map_kernel(new_cr3);

    // mapea espacio de tarea en el cr3 de la tarea
    for (size_t i = 0; i < pages; i++) {
        mmu_map_page(new_cr3, tasks_memory, phy_start, 6); 
        tasks_memory += PAGE_SIZE;  // 4096 = 4kb = tamaño pagina
        phy_start += PAGE_SIZE;
    }

    
    lcr3(new_cr3);
    // breakpoint();
    
    // creamos punteros para copiar el codigo
    char *ptr_code_page = (char *)code_start;
    char *ptr_virt_page = (char *)TASK_CODE_VIRTUAL;

    // copiamos codigo
    for (size_t i = 0; i < PAGE_SIZE * 4; i++) {
        ptr_virt_page[i] = ptr_code_page[i];
    }

    lcr3(cr3);
    tlbflush();

    return new_cr3;
}

void mmu_map_kernel(paddr_t cr3) {
    paddr_t memory_kernel = 0;
    for (int i = 0; i < 1024; i++) {
        mmu_map_page(cr3, memory_kernel, memory_kernel, 2);
        memory_kernel += PAGE_SIZE;
    }
    tlbflush();
}

void mmu_unmap_kernel(paddr_t cr3) {
    paddr_t memory_kernel = 0;
    for (int i = 0; i < 1024; i++) {
        mmu_unmap_page(cr3, memory_kernel);
        memory_kernel += PAGE_SIZE;
    }
    tlbflush();
}

void mmu_init_task_meeseeks_dir(paddr_t phy_start, paddr_t code_start, paddr_t tasks_virt_memory){
    uint32_t cr3 = rcr3();

    // creamos punteros para luego copiar el codigo
    char *ptr_code_page = (char *)code_start;
    char *ptr_virt_page = (char *)tasks_virt_memory;

    // mapea espacio de tarea
    for (size_t i = 0; i < 2; i++) {
        mmu_map_page(cr3, tasks_virt_memory, phy_start, 6);
        tasks_virt_memory += PAGE_SIZE;
        phy_start += PAGE_SIZE;
    }

    // copiamos codigo
    for (int i = 0; i < PAGE_SIZE * 2; i++) {
        ptr_virt_page[i] = ptr_code_page[i];
    }
    tlbflush();
}




void mmu_remap_meeseek(paddr_t new_phy, paddr_t virt) {

  paddr_t backup_phy = new_phy;
  paddr_t cr3 = rcr3();

  // Creamos punteros para luego copiar el codigo
  char *ptr_virt = (char *)virt;
  char *ptr_temp = (char *)new_phy/*new_phy*/;

  // mapea con identity mapping la posicion fisica para poder copiar el codigo
  for (int i = 0; i < 2; i++) {
    mmu_map_page(cr3, new_phy, new_phy, 2);
    new_phy += PAGE_SIZE;
  }

  // Copiamos codigo
  for (int i = 0; i < PAGE_SIZE * 2; i++) {
    ptr_temp[i] = ptr_virt[i];
  }
  
  
  // Desmapeamos temporaria 
  new_phy = backup_phy;
  for (int i = 0; i < 2; i++) {
    mmu_unmap_page(cr3, new_phy);
    new_phy += PAGE_SIZE;
  }

  // Mapeamos la nueva dirección del meeseek
  new_phy = backup_phy;
  for (int i = 0; i < 2; i++) {
    mmu_map_page(cr3, virt, new_phy, 2);
    new_phy += PAGE_SIZE;
    virt += PAGE_SIZE;
  }
  tlbflush();
}




/*




*/