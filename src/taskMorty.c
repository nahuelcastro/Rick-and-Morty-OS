#include "syscall.h"
#include "i386.h"


void meeseks1_func(void);


void task(void) {

  syscall_meeseeks((uint32_t)&meeseks1_func, 41, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 1, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 11, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 21, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 31, 25);

  syscall_meeseeks((uint32_t)&meeseks1_func, 51, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 61, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 71, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 56, 11);
  syscall_meeseeks((uint32_t)&meeseks1_func, 23, 8);

  //syscall_meeseeks((uint32_t)&meeseks1_func, 67, 27);
  
  while (1) {
    __asm volatile("nop");
  }
}

// void meeseks1_func(void) {
//   // Do Nothing!
//   while (1) {
//     __asm volatile("nop");
//   }


void meeseks1_func(void){
  
  while (1) {
   // __asm volatile("nop");
    for (int i = 0; i < 6; i++) {
      syscall_move(4,0);
    }
    syscall_move(0, 1);
    }
//}

  int8_t deltax, deltay;

  while (true) {
    syscall_look(&deltax, &deltay);
    syscall_move(deltax, deltay);
    continue;
    while (deltax < 0) {
      syscall_move(-1, 0);
      deltax++;
    }
    while (deltay < 0) {
      syscall_move(0, -1);
      deltay++;
    }
    while (deltax > 0) {
      syscall_move(1, 0);
      deltax--;
    }
    while (deltay > 0) {
      syscall_move(0, 1);
      deltay--;
    }
  }
}
