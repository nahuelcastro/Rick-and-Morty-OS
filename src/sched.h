/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"
#include "gdt.h" 
#include "tss.h" 
#include "i386.h" 
#include "screen.h"
#include "idt.h"
#include "colors.h"
#include "idt.h"


void sched_init();
uint16_t sched_next_task();
void desactivar_tarea();
uint16_t sched_idle();
void reiniciarCiclo();
void modo_debug(void);


extern uint16_t tareaActual;
extern uint16_t tareaActualAnterior;
extern uint16_t index;
extern player_t  ultimoJugador;
extern bool exception;
extern bool modoDebug;

typedef struct sched{
  info_task_t* info_task;
} sched_t;

extern sched_t sched[PLAYERS][11];

#endif //  __SCHED_H__
