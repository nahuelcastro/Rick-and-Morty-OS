/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"
//#include "tss.c"

uint16_t ultimoJugador = 0;
uint16_t jugadorActual = 1;
uint16_t index;
uint16_t tareaActual;
// int player_idx_gdt[35];

void sched_init(void) {
  index = 16;
  tareaActual = index;

  // for (size_t i = 0; i < 35; i++){
  //   player_idx_gdt[i] = -1;
  // }

}

uint16_t sched_next_task(void) {

  jugadorActual = 1 - ultimoJugador;

  for (int i = 0; i < GDT_COUNT-index-1; i++){

    bool esUnJugador = player_idx_gdt[index+1] == 1 || player_idx_gdt[index+1] == 0;
    bool esOtroJugador = (player_idx_gdt[index+1] != player_idx_gdt[tareaActual]) & esUnJugador;
    bool estaPresente = gdt[index+1].p == 1;
    if(estaPresente & esOtroJugador){
      index++;
      tareaActual = index;
      return ((index << 3)/* + 3*/); //! ver si hace falta sumarle 3 por los niveles de privilegio
    }else{
      index++;
    }
  }
  return ((tareaActual << 3)/* + 3*/);
}
