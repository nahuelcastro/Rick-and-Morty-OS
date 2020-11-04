/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones de rutinas para generacion de numeros aleatorios.
*/
#include "stdint.h"

// La semilla se encuentra en el archivo "seed.c"
// Para generar una nueva semilla, usar "make seed && make image"
// La semilla inicial es fija para una imagen de kernel dada,
// pero se puede sobrescribir usando la funcion srand.
extern uint64_t seed;

void srand(uint64_t s) { seed = s - 1; }

uint32_t rand(void) {
  seed = 6364136223846793005ULL * seed + 1;
  return seed;
}
