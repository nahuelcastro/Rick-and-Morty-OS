
// //TEST MODO DEBUG

// #include "stddef.h"
// #include "syscall.h"
// #include "i386.h"

// void meeseks1_func(void);

// void A(uint32_t llamado);
// void B(uint32_t llamado);
// void C(uint32_t llamado);

// uint32_t MAX_LLAMADOS = 10;
// uint32_t llamado = 0;

// void task(void) {
//   while (1) {
//     syscall_meeseeks((uint32_t)&meeseks1_func, 0, 0);
//     llamado++;
//   }
// }

// void meeseks1_func(void) {
//   A(llamado);
//   while(1){
//     __asm volatile("nop");
    
//   }
// }


// void A(uint32_t llamado){
//   if(llamado >= MAX_LLAMADOS){
//     uint8_t *p = 0;
//     p[0] = 42;
//   }else{
//     B(llamado+1);
//   }
// }

// void B(uint32_t llamado){
//   if(llamado >= MAX_LLAMADOS){
//     uint8_t *p = 0;
//     p[0] = 42;
//   }else{
//     C(llamado+1);
//   }
// }

// void C(uint32_t llamado){
//   if(llamado >= MAX_LLAMADOS){
//     uint8_t *p = 0;
//     p[0] = 42;
//   }else{
//     A(llamado+1);
//   }
// }

















///////////////////////////////////////////////////////////////////////////////////
// CODIGO ANTERIOR

#include "stddef.h"
#include "syscall.h"
#include "i386.h"
#include "game.h"

void meeseks1_func(void);


void task(void) {
  for (int row = 39; row > 0; row--) {
    for (int col = 79; col > 0; col--) {
      uint32_t res = 0;
      while (res == 0) {
        res = syscall_meeseeks((uint32_t)&meeseks1_func, col, row);
        // breakpoint();
      }
    }
}


  while (1) {
    __asm volatile("nop");
  }
}


void meeseks1_func(void){
  while (0) {
      for (int i = 0; i < 80; i++) {
        syscall_move(-1,0);

      }
      syscall_move(-1,0);
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

