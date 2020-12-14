#include "stddef.h"
#include "syscall.h"
#include "i386.h"

void meeseks1_func(void);
void meeseks2_func(void);
void meeseks3_func(void);


void task(void) {
  syscall_meeseeks((uint32_t)&meeseks1_func, 79, 1); // 19
  syscall_meeseeks((uint32_t)&meeseks1_func, 15, 5); // 21
  syscall_meeseeks((uint32_t)&meeseks1_func, 25, 5); // 23
  syscall_meeseeks((uint32_t)&meeseks1_func, 35, 5); // 25
  syscall_meeseeks((uint32_t)&meeseks1_func, 45, 5); // 27
 // 19
  syscall_meeseeks((uint32_t)&meeseks1_func, 75, 5); // 29
  syscall_meeseeks((uint32_t)&meeseks1_func, 5, 23); // 31
  syscall_meeseeks((uint32_t)&meeseks1_func, 55, 5); // 33
  syscall_meeseeks((uint32_t)&meeseks1_func, 65, 5); // 35  puta
  // syscall_meeseeks((uint32_t)&meeseks1_func, 66, 5); // 35  puta
  syscall_meeseeks((uint32_t)&meeseks1_func, 72, 12);// 37

  // breakpoint();



  // syscall_meeseeks((uint32_t)&meeseks1_func, 41, 25);  //! hecho por mi
  //syscall_meeseeks((uint32_t)&meeseks2_func, 6, 6);
  
  // breakpoint();
  

  while (1) {
    // __asm volatile("nop");
    // syscall_meeseeks((uint32_t)&meeseks1_func, 6, 6);
  }
}


void meeseks1_func(void){
  while (0) {
    // __asm volatile("nop");
      for (int i = 0; i < 20; i++) {
        syscall_move(1,0);
        // int a = 8;
        // a=a;
      }
      // syscall_move(1, 1);
      // syscall_move(0, 1);
      // syscall_move(0, 1);
      // syscall_move(1, -1);
  }


  int8_t deltax, deltay;

  while (1) {

    
    syscall_look(&deltax, &deltay);

    // print_dec(deltay, 4, 8, 30, WHITE_RED);
    // print_dec(deltax, 4, 8, 40, WHITE_RED);

    if(deltax == 0 && deltay == 0){
    }

    // continue;
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

  } // while
}