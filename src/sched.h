/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"
#include "gdt.h" // Lo agregó Maxi
#include "tss.h" // Lo agregamoo
#include "i386.h" // Lo agregamoo
#include "screen.h"
#include "idt.h"
#include "colors.h"


void sched_init();
uint16_t sched_next_task();
void desactivar_tarea();
uint16_t sched_idle();
void reiniciarCiclo();


extern uint16_t tareaActual;
extern uint16_t tareaActualAnterior;
extern uint16_t index;
extern player_t  ultimoJugador;

typedef struct sched{
  info_task_t info_task;
  bool p;
  uint8_t p_loop_sched; // se peude reutilizar de info_Task

} sched_t;

extern sched_t sched[PLAYERS][11];  // 11 hayq ue definir como max cant task for player

#endif //  __SCHED_H__
