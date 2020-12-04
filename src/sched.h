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

extern uint16_t tareaActual;
extern uint16_t index;

#endif //  __SCHED_H__
