/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  Definicion de llamadas a sistema, para usar desde las tareas.
*/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "stdbool.h"
#include "stdint.h"

#define LS_INLINE static __inline __attribute__((always_inline))

/*
 * Syscalls
 */

LS_INLINE uint32_t syscall_move(int32_t x, int32_t y) {
  int32_t ret;
  __asm__ volatile("int $123"  /* make the request to the OS */
                   : "=a"(ret) /* return value in eax ("a") */
                   : "a"(x),   /* pass in ebx ("a") arg "x"*/
                     "b"(y)    /* pass in ebx ("b") arg "y"*/
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
  return ret;
}

LS_INLINE void syscall_look(int8_t* x, int8_t* y) {
  __asm__ volatile("int $100"  /* make the request to the OS */
                   : "=a"(*x), /* delta x en eax */
                     "=b"(*y)  /* delta y en ebx */
                   :
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
}

LS_INLINE void syscall_use_portal_gun(void) {
  __asm__ volatile("int $89" /* make the request to the OS */
                   :
                   :
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
}

LS_INLINE uint32_t syscall_meeseeks(uint32_t code_start, uint32_t x,
                                    uint32_t y) {
  uint32_t ret;
  __asm__ volatile("int $88" /* make the request to the OS */
                   : "=a"(ret)
                   : "a"(code_start), // eax: inicio de codigo
                     "b"(x),          // ebx: posicion x
                     "c"(y)           // ecx: posicion y
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */

  return ret;
}

#endif //  __SYSCALL_H__