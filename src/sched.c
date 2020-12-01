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

void sched_init(void)
{
  index = 16;
  tareaActual = index;
}

uint16_t sched_next_task(void)
{

  for (int i = 0; i < GDT_COUNT - 1; i++)
  { /// TOCAR EL I
    if (index == GDT_COUNT)
    {
      index = 15;
    }

    bool esUnJugador = player_idx_gdt[index + 1] == 1 || player_idx_gdt[index + 1] == 0;
    bool esOtroJugador = (player_idx_gdt[index + 1] != player_idx_gdt[tareaActual]) && esUnJugador;
    bool estaPresente = gdt[index + 1].p == 1;
    bool estaActiva = tareasActivas[index + 1];

    if (estaPresente && esOtroJugador && estaActiva)
    {
      index++;
      tareaActual = index;
      return (index << 3); //! ver si hace falta sumarle 3 por los niveles de privilegio
    }
    else
    {
      index++;
    }
  }

  return (tareaActual << 3);
}

void desactivar_tarea(){
  if(tareaActual == 17 || tareaActual == 18){
    // terminar juego
  }

  tareasActivas[tareaActual] = false;

}


void modo_debug(void){
  15
}