/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"
#include "i386.h"



#define IDX_TSS_INIT 15
#define IDX_TSS_IDLE 16
#define IDLE_TASK_CODE 0x18000 //
#define IDX_CODE_LVL_0 0x50
#define IDX_DATO_LVL_0 0x58    //0101 1000 58   
#define IDX_CODE_LVL_3 0x60   //0110 0000  12
#define IDX_DATO_LVL_3 0x68   //0110 1000  13
#define PAGE_SIZE 4096
#define TASK_VIRTUAL_DIR 0x1D00000

tss_t tss_initial;
tss_t tss_idle;
tss_t tss_new_task;

task_type_t player_idx_gdt[35];

void init_tss(void) {

  for (int i = 0; i < 35; i++){
    player_idx_gdt[i]=Meeseeks;
  }
  uint32_t base = (uint32_t) &tss_initial;
  tss_gdt_entry_init(IDX_TSS_INIT, base);

}

void init_idle(){

  uint32_t base = (uint32_t) &tss_idle;  
  tss_gdt_entry_init(IDX_TSS_IDLE, base);

  tss_idle.ptl = 0;
  tss_idle.unused0 = 0;
  tss_idle.esp0 = KERNEL_STACK; 
  tss_idle.ss0 = IDX_DATO_LVL_0;
  tss_idle.unused1 = 0;
  tss_idle.esp1 = 0;
  tss_idle.ss1 = 0;
  tss_idle.unused2 = 0;
  tss_idle.esp2 = 0;
  tss_idle.ss2 = 0;
  tss_idle.unused3 = 0;
  tss_idle.cr3 = KERNEL_PAGE_DIR;
  tss_idle.eip = IDLE_TASK_CODE;
  tss_idle.eflags = 0x202; 
  tss_idle.eax = 0;
  tss_idle.ecx = 0;
  tss_idle.edx = 0;
  tss_idle.ebx = 0;
  tss_idle.esp = KERNEL_STACK;
  tss_idle.ebp = 0;
  tss_idle.esi = 0;
  tss_idle.edi = 0;
  tss_idle.es = IDX_DATO_LVL_0;
  tss_idle.unused4 = 0;
  tss_idle.cs = IDX_CODE_LVL_0;
  tss_idle.unused5 = 0;
  tss_idle.ss = IDX_DATO_LVL_0;
  tss_idle.unused6 = 0;
  tss_idle.ds = IDX_DATO_LVL_0;
  tss_idle.unused7 = 0;
  tss_idle.fs = IDX_DATO_LVL_0;
  tss_idle.unused8 = 0;
  tss_idle.gs = IDX_DATO_LVL_0;
  tss_idle.unused9 = 0;
  tss_idle.ldt = 0;
  tss_idle.unused10 = 0;
  tss_idle.dtrap = 0;
  tss_idle.iomap = 0xFFFF; 

}


void tss_gdt_entry_init(uint32_t index, uint32_t tss) {
    
    gdt[index].limit_15_0 = 0x0067;
    gdt[index].base_15_0 = tss & 0xFFFF;
    gdt[index].base_23_16 = (tss >> 16) & 0xFF;
    gdt[index].type = 0x9;
    gdt[index].s = 0x0;
    gdt[index].dpl = 0x0;
    gdt[index].p = 0x1;
    gdt[index].limit_19_16 = 0x0;
    gdt[index].avl = 0x1;
    gdt[index].l = 0x0;
    gdt[index].db = 0x0;
    gdt[index].g = 0x0;
    gdt[index].base_31_24 = (tss >> 24);

}



//uint32_t next_free_task_space = 0x3C000; // 0x40000 - 16384 (4 paginas de 4KB)
uint32_t next_free_gdt_idx = 16;

void next_free_tss() {
  next_free_gdt_idx++;
//  next_free_task_space+=4096*pages;

}



void tss_creator(paddr_t code_start, int player){    // code_start=0x10000 (Rick) o 0x14000 (Morty)  Si tareas Rick o Morty, jugador=1 (true)

  paddr_t next_free_task_space;
  if(player == 1){                                   // 1 = Rick
     next_free_task_space = 0x1D00000;
  }else{                                             // 0 = Morty
     next_free_task_space = 0x1D04000;
  }

  breakpoint();
//ROMPE ACÁ! Chequear posicion
  paddr_t page_dir = mmu_init_task_dir(next_free_task_space, code_start, 4); // ver si esta bien lo de la posicion 0x40000, para mi son las mismas posicones que ej 5
  paddr_t stack_level_0 = mmu_next_free_kernel_page();  //usa pagina nueva del area libre kernel

 // tss_t tss_new_task;

  
  next_free_tss(); // actualiza
  player_idx_gdt[next_free_gdt_idx] = player; // nos indica a que jugador pertenece el indice de la gdt
 
  uint32_t base = (uint32_t) &tss_new_task; 
  tss_gdt_entry_init(next_free_gdt_idx,base);

  gdt[next_free_gdt_idx].dpl = 3;
  
    tss_new_task.ptl = 0;
    tss_new_task.unused0 = 0;
    tss_new_task.esp0 = stack_level_0 + PAGE_SIZE;
    tss_new_task.ss0 = IDX_DATO_LVL_0;
    tss_new_task.unused1 = 0;
    tss_new_task.esp1 = 0;
    tss_new_task.ss1 = 0;
    tss_new_task.unused2 = 0;
    tss_new_task.esp2 = 0;
    tss_new_task.ss2 = 0;
    tss_new_task.unused3 = 0;
    tss_new_task.cr3 = page_dir;
    tss_new_task.eip = TASK_VIRTUAL_DIR;
    tss_new_task.eflags = 0x202;
    tss_new_task.eax = 0;
    tss_new_task.ecx = 0;
    tss_new_task.edx = 0;
    tss_new_task.ebx = 0;
    tss_new_task.esp = TASK_VIRTUAL_DIR + 4 * PAGE_SIZE;
    tss_new_task.ebp = TASK_VIRTUAL_DIR + 4 * PAGE_SIZE; 
    tss_new_task.esi = 0;
    tss_new_task.edi = 0;
    tss_new_task.es  = IDX_DATO_LVL_3;
    tss_new_task.unused4 = 0;
    tss_new_task.cs = IDX_CODE_LVL_3;
    tss_new_task.unused5 = 0;
    tss_new_task.ss = IDX_DATO_LVL_3;
    tss_new_task.unused6 = 0;
    tss_new_task.ds = IDX_DATO_LVL_3;
    tss_new_task.unused7 = 0;
    tss_new_task.fs = IDX_DATO_LVL_3;
    tss_new_task.unused8 = 0;
    tss_new_task.gs = IDX_DATO_LVL_3;
    tss_new_task.unused9 = 0;
    tss_new_task.ldt = 0;
    tss_new_task.unused10 = 0;
    tss_new_task.dtrap = 0;
    tss_new_task.iomap = 0xFFFF;

}



// void tss_creator(paddr_t code_start,bool player){    // code_start=0x10000 (Rick) o 0x14000 (Morty)  Si tareas Rick o Morty, jugador=1 (true)
//   if(player){
//     next_free_tss(4);
//   }else{
//     next_free_tss(2); // parece que es para despues con las tareas meesekss
//   }
//   paddr_t page_dir = mmu_init_task_dir(next_free_task_space,code_start,4);  // ver si esta bien lo de la posicion 0x40000, para mi son las mismas posicones que ej 5

//   paddr_t stack_level_0 = mmu_next_free_kernel_page();  //usa pagina nueva del area libre kernel

//   tss_t tss_new_task={
//     .ptl = 0,
//     .unused0 = 0,
//     .esp0 = stack_level_0 + PAGE_SIZE,
//     .ss0 = IDX_DATO_LVL_0,
//     .unused1 = 0,
//     .esp1 = 0,
//     .ss1 = 0,
//     .unused2 = 0,
//     .esp2 = 0,
//     .ss2 = 0,
//     .unused3 = 0,
//     .cr3 = page_dir,
//     .eip = next_free_task_space,
//     .eflags = 0x202,
//     .eax = 0,
//     .ecx = 0,
//     .edx = 0,
//     .ebx = 0,
//     .esp = next_free_task_space + 4 * PAGE_SIZE,
//     .ebp = next_free_task_space + 4 * PAGE_SIZE, 
//     .esi = 0,
//     .edi = 0,
//     .es  = IDX_DATO_LVL_3,
//     .unused4 = 0,
//     .cs = IDX_CODE_LVL_3,
//     .unused5 = 0,
//     .ss = IDX_DATO_LVL_3,
//     .unused6 = 0,
//     .ds = IDX_DATO_LVL_3,
//     .unused7 = 0,
//     .fs = IDX_DATO_LVL_3,
//     .unused8 = 0,
//     .gs = IDX_DATO_LVL_3,
//     .unused9 = 0,
//     .ldt = 0,
//     .unused10 = 0,
//     .dtrap = 0,
//     .iomap = 0xFFFF,          // VER QUE ONDA
//   };
//   tss_gdt_entry_init(next_free_gdt_idx, tss_new_task);
// }
