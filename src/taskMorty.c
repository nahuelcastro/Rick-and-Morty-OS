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

  syscall_meeseeks((uint32_t)&meeseks1_func,79,0);
  syscall_meeseeks((uint32_t)&meeseks1_func, 1, 25); 
  syscall_meeseeks((uint32_t)&meeseks1_func, 11, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 21, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 31, 25);

  syscall_meeseeks((uint32_t)&meeseks1_func, 51, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 61, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 71, 25);
  syscall_meeseeks((uint32_t)&meeseks1_func, 56, 11);
  syscall_meeseeks((uint32_t)&meeseks1_func, 23, 8);
  syscall_meeseeks((uint32_t)&meeseks1_func, 67, 27);
  
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
    //for (int i = 0; i < 100; i++) {
      // int8_t deltax, deltay;
      // syscall_look(&deltax, &deltay);
      // syscall_move(deltax,deltay);
      //syscall_move(0, 4);
     syscall_move(0, 1);
    }
  //}
}


  //int8_t deltax, deltay;

  // while (0) {

    
  //   syscall_move(deltax, deltay);
  //   continue;
  //   while (deltax < 0) {
  //     syscall_move(-1, 0);
  //     deltax++;
  //   }
  //   while (deltay < 0) {
  //     syscall_move(0, -1);
  //     deltay++;
  //   }
  //   while (deltax > 0) {
  //     syscall_move(1, 0);
  //     deltax--;
  //   }
  //   while (deltay > 0) {
  //     syscall_move(0, 1);
  //     deltay--;
  //   }
  // }

