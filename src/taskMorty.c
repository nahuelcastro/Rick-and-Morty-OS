#include "syscall.h"
#include "i386.h"
#include "colors.h"
#include "screen.h"

#include "game.h"
#include "mmu.h"
#include "prng.h"
#include "sched.h"
#include "stdint.h"
#include "types.h"


void meeseks11_func(void);
void meeseks77_func(void);

void task(void) {
  for (int row = 0; row < 39; row++) {
    for (int col = 0; col < 79; col++) {
      uint32_t res = 0;
      while (res == 0 ) {
        res = syscall_meeseeks((uint32_t)&meeseks77_func, col, row);
      }
    }
}


  while (1) {
    __asm volatile("nop");
  }
}



void meeseks77_func(void){
  while (0) {
      for (int i = 0; i < 80; i++) {
        syscall_move(0,-1);

      }
      syscall_move(0,-1);
  }


  int8_t deltax, deltay;

  while (1) {   
    syscall_look(&deltax, &deltay);

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

    syscall_use_portal_gun();

  } 
}



void meeseks11_func(void){
  while (0) {
      for (int i = 0; i < 80; i++) {
        syscall_move(0,-1);

      }
      syscall_move(0,-1);
  }


  int8_t deltax, deltay;

  while (1) {   
    syscall_look(&deltax, &deltay);

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

    syscall_use_portal_gun();

  } 
}
