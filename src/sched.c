/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"
//#include "tss.c"

uint16_t index = 0;


void sched_init(void) {
  index = 16;
} 

uint16_t sched_next_task(void) {

  uint16_t tareaActual = index; 
 
  for (int i = 0; i < GDT_COUNT-index-1; i++){
    bool esUnJugador = player_idx_gdt[index+1] == 1 || player_idx_gdt[index+1] == 0;
    bool esOtroJugador = (player_idx_gdt[index+1] != player_idx_gdt[tareaActual]) & esUnJugador;
    bool estaPresente = gdt[index+1].p == 1;
    if(estaPresente & esOtroJugador){
      return (++index << 3);
    }else{
      index++;
    }
  }  
  return (tareaActual << 3);   
}





// uint16_t sched_next_task(void) {

//   uint16_t tareaActual = index;
//   //Conseguismos direccion de la tss actual
//   uint32_t base_actual_task = (gdt[index].base_31_24 << 24) & 0xFF;
//   base_actual_task += (gdt[index].base_23_16 << 16) & 0xFF;
//   base_actual_task += gdt[index].base_15_0 & 0xFFFF;
//   // Conseguimos eip de la tarea actual
//   uint32_t eip_actual = base_actual_task + 32;

//   uint32_t base_next_task;
//   uint32_t eip_next;
  
//   for (size_t i = 0; i < GDT_COUNT; i++){

//     //Conseguimos direccion de la tss siguiente
//     base_next_task = (gdt[index+1].base_31_24 << 24) & 0xFF;
//     base_next_task += (gdt[index+1].base_23_16 << 16) & 0xFF;
//     base_next_task += gdt[index+1].base_15_0 & 0xFFFF;

//     eip_next = base_next_task + 32;

//     if(gdt[index+1].p == 1 && eip_actual != eip_next){
//       index++;
//       return index;
//     }else{
//       index++;
//     }
//   }
//   return tareaActual;

// }