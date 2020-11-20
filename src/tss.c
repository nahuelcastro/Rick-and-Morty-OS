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


#define GDT_IDX_TSS_INIT 15
#define GDT_IDX_TSS_IDLE 16
#define IDLE_TASK_CODE 0x18000
#define CODE_KERNEL_SEG 0x50
#define DATA_KERNEL_SEG 0x58




void init_tss_inicial(void) {
  
  tss_t tss_initial = {0};
  tss_gdt_entry_init(GDT_IDX_TSS_INIT, tss_initial);

}


void init_tss_idle(){

tss_gdt_entry_init(GDT_IDX_TSS_IDLE, tss_idle);

  tss_t tss_idle = {
    .ptl = 0,
    .unused0 = 0,
    .esp0 = KERNEL_STACK,
    .ss0 = DATA_KERNEL_SEG,   //segmento de datos del kernel shifteado 3
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
    .es  = DATA_KERNEL_SEG,
    .unused4 = 0,
    .cs = CODE_KERNEL_SEG,  //segmento de codigo del kernel shifteado 3
    .unused5 = 0,
    .ss = DATA_KERNEL_SEG,
    .unused6 = 0,
    .ds = DATA_KERNEL_SEG,
    .unused7 = 0,
    .fs = DATA_KERNEL_SEG,
    .unused8 = 0,
    .gs = DATA_KERNEL_SEG,
    .unused9 = 0,
    .ldt = 0,
    .unused10 = 0,
    .dtrap = 0,
    .iomap = 0xFFFF,          // VER QUE ONDA
  };
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




