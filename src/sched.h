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

void sched_init();
uint16_t sched_next_task();

#endif //  __SCHED_H__
