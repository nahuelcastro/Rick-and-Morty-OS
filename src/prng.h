/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones de generacion de numeros aleatorios.
*/

#ifndef __PRNG_H__
#define __PRNG_H__

#include "stdint.h"

void srand(uint64_t s);
uint32_t rand(void);

#endif //  __PRNG_H__