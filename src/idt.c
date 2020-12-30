/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"
#include "defines.h"
#include "isr.h"
#include "screen.h"
#include "sched.h"

int ultExcepcion=260;

idt_entry_t idt[255] = {0};

idt_descriptor_t IDT_DESC = {sizeof(idt) - 1, (uint32_t)&idt};


#define CODE_SEL_0 0x50 
#define INTERRUPT_0_ATTRS 0x8E00
#define INTERRUPT_3_ATTRS 0xEE00

#define IDT_ENTRY(numero)                                                              \
  idt[numero].offset_15_0 = (uint16_t)((uint32_t)(&_isr##numero) & (uint32_t)0xFFFF);  \
  idt[numero].segsel = (uint16_t) CODE_SEL_0;                                          \
  idt[numero].attr = (uint16_t)INTERRUPT_0_ATTRS;                                      \
  idt[numero].offset_31_16 = (uint16_t)((uint32_t)(&_isr##numero) >> 16 & (uint32_t)0xFFFF);


#define IDT_ENTRY_3(numero)                                                              \
  idt[numero].offset_15_0 = (uint16_t)((uint32_t)(&_isr##numero) & (uint32_t)0xFFFF);  \
  idt[numero].segsel = (uint16_t) CODE_SEL_0;                                          \
  idt[numero].attr = (uint16_t)INTERRUPT_3_ATTRS;                                      \
  idt[numero].offset_31_16 = (uint16_t)((uint32_t)(&_isr##numero) >> 16 & (uint32_t)0xFFFF);

void idt_init() {

  IDT_ENTRY(0);
  IDT_ENTRY(1);
  IDT_ENTRY(2);
  IDT_ENTRY(3);
  IDT_ENTRY(4);
  IDT_ENTRY(5);
  IDT_ENTRY(6);
  IDT_ENTRY(7);
  IDT_ENTRY(8);
  IDT_ENTRY(9);
  IDT_ENTRY(10);
  IDT_ENTRY(11);
  IDT_ENTRY(12);
  IDT_ENTRY(13);
  IDT_ENTRY(14);
  IDT_ENTRY(15);
  IDT_ENTRY(16);
  IDT_ENTRY(17);
  IDT_ENTRY(18);
  IDT_ENTRY(19);
  IDT_ENTRY(32);
  IDT_ENTRY(33);

  IDT_ENTRY_3(88);
  IDT_ENTRY_3(89);
  IDT_ENTRY_3(100);
  IDT_ENTRY_3(123);
}


void imprimir_excepcion(int codigo){
  
  if (codigo == 0) { print("Divide Error(0)", 22, 2, C_FG_LIGHT_GREEN);}

  if (codigo == 1) { print("RESERVED (1)",22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 2) { print("NMI Interrupt (2)",22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 3) { print("Breackpoint (3)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 4) { print("Overflow (4)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 5) { print("BOUND Range Exceeded (5)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 6) { print("Invalid Opcode (Undifined Opcode) (6)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 7) { print("Device Not Avaiable (No Math Coprocessor) (7) ", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 8) { print("Double Fault (8)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 9) { print("Coprocessor Segment Overrun (reserved) (9)", 22, 2, C_FG_LIGHT_GREEN);}

  if (codigo == 10){ print("Invalid TSS (10)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 11){ print("Segment Not Present (11)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 12){ print("Stack-Segment Fault (12)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 13){ print("General Protection (13)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 14){ print("Page Fault (14)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 15){ print("(Intel reserved. Do Not Use.) (15)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 16){ print("x87 FPU Floating-Point Error (Math Fault) (16)", 22,0, C_FG_LIGHT_GREEN);}

  if (codigo == 17){ print("Alignment Check (17)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 18){print("Machine Check (18)", 22,2, C_FG_LIGHT_GREEN);}

  if (codigo == 19){ print("SIMD Floating-Point Exception (19)", 22,2, C_FG_LIGHT_GREEN);}

}



