#include "stddef.h"
#include "syscall.h"
#include "i386.h"

void meeseks1_func(void);
void meeseks2_func(void);
void meeseks3_func(void);


void task(void) {
  syscall_meeseeks((uint32_t)&meeseks1_func, 5, 5);
  syscall_meeseeks((uint32_t)&meeseks1_func, 15, 5);
  syscall_meeseeks((uint32_t)&meeseks1_func, 25, 5);
  syscall_meeseeks((uint32_t)&meeseks1_func, 35, 5);
  syscall_meeseeks((uint32_t)&meeseks1_func, 45, 5);

  syscall_meeseeks((uint32_t)&meeseks1_func, 75, 5);
  syscall_meeseeks((uint32_t)&meeseks1_func, 5, 23);
  syscall_meeseeks((uint32_t)&meeseks1_func, 55, 5);
  syscall_meeseeks((uint32_t)&meeseks1_func, 65, 5);
  syscall_meeseeks((uint32_t)&meeseks1_func, 72, 12);

  // syscall_meeseeks((uint32_t)&meeseks1_func, 41, 25);  //! hecho por mi
  //syscall_meeseeks((uint32_t)&meeseks2_func, 6, 6);
  
  // breakpoint();
  

  while (1) {
    __asm volatile("nop");
    // syscall_meeseeks((uint32_t)&meeseks2_func, 6, 6);
  }
}


void meeseks1_func(void){
  
  while (1) {
    // __asm volatile("nop");
      for (int i = 0; i < 6; i++) {
        syscall_move(0,3);
      }
      syscall_move(1, 0);
    }
}

  

// void meeseks2_func(void) {
//   while (1) {
//     for (int i = 0; i < 80; i++) {
//       syscall_move(1, 0);
//     }
//     syscall_move(0, 1);
//   }
// }

// void meeseks3_func(void){
//   while (1) {
//     // __asm volatile("nop");
//       for (int i = 0; i < 80; i++) {
//         syscall_move(0,3);
//       }
//       syscall_move(0, 0);
//     }
//   }

/*

CR2=page fault laddr=0x01d03ffc


*/