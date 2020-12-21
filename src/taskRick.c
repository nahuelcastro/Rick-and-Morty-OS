#include "stddef.h"
#include "syscall.h"
#include "i386.h"
#include "game.h"

void meeseks1_func(void);
void meeseks2_func(void);
void meeseks3_func(void);


void task(void) {
  for (int row = 0; row < 40; row++) {
    for (int col = 0; col < 80; col++) {
      uint32_t res = 0;
      while (res == 0) {
        res = syscall_meeseeks((uint32_t)&meeseks1_func, col, row);
        // breakpoint();
      }
    }
  }

    //   while(1){
    //   for (size_t i = 0; i < 80; i = i + 2){
    //     for (size_t j = 0; i < 40; j = j + 4){
    //       syscall_meeseeks((uint32_t)&meeseks1_func, i , j);// 37
    //       if(i > 70){
    //         i = 1;
    //       }
    //       if(j > 34){
    //         j = 2;
    //       }
    //     }
    //   }
    // }


  while (1) {
    __asm volatile("nop");
  }
}


void meeseks1_func(void){
  while (0) {
      for (int i = 0; i < 20; i++) {
        syscall_move(1,0);

      }
      syscall_move(0,1);
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








































// void meeseks1_func(void);

// void task(void) {
//   for (int i = 0; i < 80; i++) {
//     syscall_meeseeks((uint32_t)&meeseks1_func, i, 5);
//   }
//   while (1) {
//     __asm volatile("nop");
//   }
// }

// void meeseks1_func(void) {
//   uint8_t* kernel_free_area_start = (uint8_t*) 0x100000;
//   int i = 0;
//   while (1) {
//     kernel_free_area_start[i] = 0x0;
//     i++;
//   }
// }

