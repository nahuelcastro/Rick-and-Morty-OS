#include "syscall.h"

void meeseks1_func(void);

void task(void) {
  syscall_meeseeks((uint32_t)&meeseks1_func, 40, 25);

  while (1) {
    __asm volatile("nop");
  }
}

void meeseks1_func(void) {
  // Do Nothing!
  while (1) {
    __asm volatile("nop");
  }

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
