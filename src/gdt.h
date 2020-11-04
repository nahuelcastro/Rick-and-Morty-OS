/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de la tabla de descriptores globales
*/

#ifndef __GDT_H__
#define __GDT_H__

#include "defines.h"
#include "types.h"

typedef struct str_gdt_descriptor {
  uint16_t gdt_length;
  uint32_t gdt_addr;
} __attribute__((__packed__)) gdt_descriptor_t;

typedef struct str_gdt_entry {
  uint16_t limit_15_0;
  uint16_t base_15_0;
  uint8_t base_23_16;
  uint8_t type : 4;
  uint8_t s : 1;
  uint8_t dpl : 2;
  uint8_t p : 1;
  uint8_t limit_19_16 : 4;
  uint8_t avl : 1;
  uint8_t l : 1;
  uint8_t db : 1;
  uint8_t g : 1;
  uint8_t base_31_24;
} __attribute__((__packed__, aligned(8))) gdt_entry_t;

/* Tabla GDT */
extern gdt_entry_t gdt[];
extern gdt_descriptor_t GDT_DESC;

#endif // __GDT_H__
