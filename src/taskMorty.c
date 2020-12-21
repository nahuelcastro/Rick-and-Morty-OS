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


void meeseks1_func(void);


void task(void) {

  // for (int row = 0; row < 40; row++) {
  //   for (int col = 0; col < 80; col++) {
  //     uint32_t res = 0;
  //     while (res == 0) {
  //       res = syscall_meeseeks((uint32_t)&meeseks1_func, col, row);
  //     }
  //   }
  // }


  while(1){
    for (size_t i = 0; i < 80; i = i + 8){
      for (size_t j = 0; i < 40; j = j + 5){
        syscall_meeseeks((uint32_t)&meeseks1_func, i , j);// 37
        if(i > 70){
          i = 2;
        }
        if(j > 34){
          j = 1;
        }
      }
    }
  }



  while (1) {
    __asm volatile("nop");
  }
}



void meeseks1_func(void){
  while (1) {
      for (int i = 0; i < 20; i++) {
        syscall_move(1,0);
      }
  }


  int8_t deltax, deltay;

  while (0) {
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














































// void meeseeks_quieto_func(void);

// void task(void) {
//   for (int row = 0; row < 40; row++) {
//     for (int col = 0; col < 80; col++) {
//       uint32_t res = 0;
//       while (res == 0) {
//         res = syscall_meeseeks((uint32_t)&meeseeks_quieto_func, col, row);
//       }
//     }
//   }

//   while (1) {
//     __asm volatile("nop");
//   }
// }

// void meeseeks_quieto_func(void) {
//   int8_t x, y;
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   syscall_look(&x, &y);
//   __builtin_trap();
// }




























// void task(void) {
//   while (1) {
//     __asm volatile("nop");
//   }
// }
