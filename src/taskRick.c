#include "stddef.h"
#include "syscall.h"

void meeseks1_func(void);
void meeseks2_func(void);

void task(void) {
  syscall_meeseeks((uint32_t)&meeseks1_func, 5, 5);
  syscall_meeseeks((uint32_t)&meeseks2_func, 6, 6);

  while (1) {
    __asm volatile("nop");
  }
}

void meeseks1_func(void) {
  while (1) {
    for (int i = 0; i < 80; i++) {
      syscall_move(-1, 0);
    }
    syscall_move(0, -1);
  }
}

void meeseks2_func(void) {
  while (1) {
    for (int i = 0; i < 80; i++) {
      syscall_move(1, 0);
    }
    syscall_move(0, 1);
  }
}
