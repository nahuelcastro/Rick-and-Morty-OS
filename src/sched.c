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
