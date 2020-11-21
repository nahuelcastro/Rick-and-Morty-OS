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


#define IDX_TSS_INIT 15
#define IDX_TSS_IDLE 16
#define IDLE_TASK_CODE 0x18000
#define IDX_CODE_LVL_0 0x50
#define IDX_DATO_LVL_0 0x58
#define IDX_CODE_LVL_3 0x60   //0110 0000  12
#define IDX_DATO_LVL_3 0x68   //0110 1000  13

uint32_t next_free_task_space = 0x3C000; // 0x40000 - 16384 (4 paginas de 4KB) 
uint32_t next_free_gdt_idx = 16;


void init_tss_inicial(void) {
  
  tss_t tss_initial = {0};

  tss_gdt_entry_init(IDX_TSS_INIT, tss_initial);

}


void init_tss_idle(){

  tss_t tss_idle = {
    .ptl = 0,
    .unused0 = 0,
    .esp0 = KERNEL_STACK,
    .ss0 = IDX_DATO_LVL_0,   //segmento de datos del kernel shifteado 3
    .unused1 = 0,
    .esp1 = 0,
    .ss1 = 0,
    .unused2 = 0,
    .esp2 = 0,
    .ss2 = 0,
    .unused3 = 0,
    .cr3 = KERNEL_PAGE_DIR,
    .eip = IDLE_TASK_CODE,
    .eflags = 0x202,        //ver porque 
    .eax = 0,
    .ecx = 0,
    .edx = 0,
    .ebx = 0,
    .esp = KERNEL_STACK,
    .ebp = KERNEL_STACK,
    .esi = 0,
    .edi = 0,
    .es  = IDX_DATO_LVL_0,
    .unused4 = 0,
    .cs = IDX_CODE_LVL_0,  //segmento de codigo del kernel shifteado 3
    .unused5 = 0,
    .ss = IDX_DATO_LVL_0,
    .unused6 = 0,
    .ds = IDX_DATO_LVL_0,
    .unused7 = 0,
    .fs = IDX_DATO_LVL_0,
    .unused8 = 0,
    .gs = IDX_DATO_LVL_0,
    .unused9 = 0,
    .ldt = 0,
    .unused10 = 0,
    .dtrap = 0,
    .iomap = 0xFFFF,          // VER QUE ONDA
  };

tss_gdt_entry_init(IDX_TSS_IDLE, tss_idle);

}


void tss_gdt_entry_init(uint8_t index, tss_t tss){
  
      gdt[index].base_15_0 = (uint32_t) &tss & 0xFFFF;        // Para quedarnos con los primeros 16 bits
      gdt[index].base_23_16 = ((uint32_t) &tss >> 16) & 0xFF; // Para quedarnos con los primeros 8 bits del shifteado
      gdt[index].base_31_24 = ((uint32_t) &tss >> 24);
      gdt[index].limit_15_0 = 0x67;
      gdt[index].limit_19_16 = 0;
      gdt[index].type = 0x9;
      gdt[index].s = 0;
      gdt[index].dpl = 0;
      gdt[index].p = 0x1;
      gdt[index].avl = 0;
      gdt[index].l = 0;
      gdt[index].db = 0;
      gdt[index].g = 0;
      
}



void next_free_tss(uint8_t pages) {
  next_free_gdt_idx++;
  next_free_task_space+=4096*pages;
} 


void tss_creator(paddr_t code_start,bool jugador){    // code_start=0x10000 (Rick) o 0x14000 (Morty)  Si tareas Rick o Morty, jugador=1 (true)
  if(jugador){
    next_free_tss(4);
  }else{
    next_free_tss(2); // parece que es para despues con las tareas meesekss 
  }
  paddr_t page_dir = mmu_init_task_dir(next_free_task_space,code_start,4);  // ver si esta bien lo de la posicion 0x40000, para mi son las mismas posicones que ej 5
  
  paddr_t stack_level_0 = mmu_next_free_kernel_page;  //usa pagina nueva del area libre kernel

  tss_t tss_new_task={
    .ptl = 0,
    .unused0 = 0,
    .esp0 = stack_level_0,   // ver si es desde la base o el limite 
    .ss0 = IDX_DATO_LVL_0,   
    .unused1 = 0,
    .esp1 = 0,
    .ss1 = 0,
    .unused2 = 0,
    .esp2 = 0,
    .ss2 = 0,
    .unused3 = 0,
    .cr3 = page_dir,
    .eip = next_free_task_space,  
    .eflags = 0x202,       
    .eax = 0,
    .ecx = 0,
    .edx = 0,
    .ebx = 0,
    .esp = next_free_task_space, // dice que la pila usa el mismo espacio que la tarea (ver el tema de la base)
    .ebp = next_free_task_space, // dice que la pila usa el mismo espacio que la tarea (ver el tema de la base)
    .esi = 0,
    .edi = 0,
    .es  = IDX_DATO_LVL_3,
    .unused4 = 0,
    .cs = IDX_CODE_LVL_3,  
    .unused5 = 0,
    .ss = IDX_DATO_LVL_3,
    .unused6 = 0,
    .ds = IDX_DATO_LVL_3,
    .unused7 = 0,
    .fs = IDX_DATO_LVL_3,
    .unused8 = 0,
    .gs = IDX_DATO_LVL_3,
    .unused9 = 0,
    .ldt = 0,
    .unused10 = 0,
    .dtrap = 0,
    .iomap = 0xFFFF,          // VER QUE ONDA
  };
  tss_gdt_entry_init(next_free_gdt_idx, tss_new_task);
}



