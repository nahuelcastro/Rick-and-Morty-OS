
#include "stddef.h"
#include "syscall.h"
#include "i386.h"
#include "game.h"

void meeseks7_func_rick(void);
void meeseks6_func_rick(void);
void meeseks5_func_rick(void);
void meeseks4_func_rick(void);
void meeseks2_func_rick(void);
void meeseks3_func_rick(void);
void meeseks1_func_rick(void);


void task(void) {
  for (int col = 79; col > 0; col--) {
    for (int row = 39; row > 0; row--) {
      uint32_t res = 0;
      while (res == 0) {
        res = syscall_meeseeks((uint32_t)&meeseks1_func_rick, col, row);
        // res = res;
        
      }
    }
}


  while (1) {
    __asm volatile("nop");
  }
}

void meeseks7_func_rick(void){
  while(true){
    syscall_move(1,0);
  }
}


void meeseks1_func_rick(void){
  while (0) {
      for (int i = 0; i < 80; i++) {
        syscall_move(-1,0);

      }
      syscall_move(-1,0);
  }


  int8_t deltaxx, deltayy;

  while (1) {   
    syscall_look(&deltaxx, &deltayy);

    // deltaxx = 8;
    // deltayy = 8;

    while (deltaxx < 0) {
      syscall_move(-1, 0);
      deltaxx++;
    }
    while (deltayy < 0) {
      syscall_move(0, -1);
      deltayy++;
    }
    while (deltaxx > 0) {
      syscall_move(1, 0);
      deltaxx--;
    }
    while (deltayy > 0) {
      syscall_move(0, 1);
      deltayy--;
    }

    syscall_use_portal_gun();
  } 
}
