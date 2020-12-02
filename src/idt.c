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

int ultExcepcion=260;
idt_entry_t idt[255] = {0};

idt_descriptor_t IDT_DESC = {sizeof(idt) - 1, (uint32_t)&idt};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);

        IDT_ENTRY(19);

    }
*/

#define CODE_SEL_0 0x50 // 0x50
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

  ultExcepcion = codigo;
  print("Excepcion, ", 0, 0, C_FG_LIGHT_GREEN);
  if (codigo == 0) { print("Divide Error(0)", 41, 2, C_FG_LIGHT_GREEN);}

  if (codigo == 1) { print("RESERVED (1)",41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 2) { print("NMI Interrupt (2)",41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 3) { print("Breackpoint (3)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 4) { print("Overflow (4)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 5) { print("BOUND Range Exceeded (5)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 6) { print("Invalid Opcode (Undifined Opcode) (6)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 7) { print("Device Not Avaiable (No Math Coprocessor) (7) ", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 8) { print("Double Fault (8)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 9) { print("Coprocessor Segment Overrun (reserved) (9)", 41, 2, C_FG_LIGHT_GREEN);}

  if (codigo == 10){ print("Invalid TSS (10)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 11){ print("Segment Not Present (11)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 12){ print("Stack-Segment Fault (12)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 13){ print("General Protection (13)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 14){ print("Page Fault (14)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 15){ print("(Intel reserved. Do Not Use.) (15)", 41,2, C_FG_LIGHT_GREEN);}

  if (codigo == 16){ print("x87 FPU Floating-Point Error (Math Fault) (16)", 41,0, C_FG_LIGHT_GREEN);}

  if (codigo == 17){ print("Alignment Check (17)", 11,0, C_FG_LIGHT_GREEN);}

  if (codigo == 18){print("Machine Check (18)", 11,0, C_FG_LIGHT_GREEN);}

  if (codigo == 19){ print("SIMD Floating-Point Exception (19)", 11,0, C_FG_LIGHT_GREEN);}

  if (codigo == 33){ print("ESTAN ESCRIBIENDOOOOO!! (33)", 11,0, C_FG_LIGHT_GREEN);}

  if (codigo == 88){ print("Entramos en una tarea!! (88)", 11,0,C_FG_LIGHT_GREEN);}
}



