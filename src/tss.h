/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "gdt.h"
#include "types.h"
#include "game.h"

// extern int8_t player_idx_gdt[GDT_COUNT];
// extern bool tareasActivas[GDT_COUNT];

typedef struct str_tss {
  uint16_t ptl;
  uint16_t unused0;
  uint32_t esp0;
  uint16_t ss0;
  uint16_t unused1;
  uint32_t esp1;
  uint16_t ss1;
  uint16_t unused2;
  uint32_t esp2;
  uint16_t ss2;
  uint16_t unused3;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint16_t es;
  uint16_t unused4;
  uint16_t cs;
  uint16_t unused5;
  uint16_t ss;
  uint16_t unused6;
  uint16_t ds;
  uint16_t unused7;
  uint16_t fs;
  uint16_t unused8;
  uint16_t gs;
  uint16_t unused9;
  uint16_t ldt;
  uint16_t unused10;
  uint16_t dtrap;
  uint16_t iomap;
} __attribute__((__packed__, aligned(8))) tss_t;


void init_tss(void);

void init_idle();

void tss_gdt_entry_init(uint32_t index, uint32_t base, int dpl);

extern tss_t* TSSs[35];
extern uint32_t gdt_index;

typedef struct meeseek{
  uint8_t p;
  uint8_t gdt_index;
  coordenadas coord;
  bool used_portal_gun;
  // tss_t tss;
} meeseek_t;


typedef struct info_task{
  bool active;
  uint8_t idx_gdt;
  uint8_t idx_msk;
  uint8_t flag_loop;
  player_t player;
  uint8_t clock;
} info_task_t;


extern info_task_t info_task[GDT_COUNT];
extern uint8_t cant_meeseeks[PLAYERS];                 // cant_meeseeks[player]
extern meeseek_t meeseeks[PLAYERS][MAX_CANT_MEESEEKS]; // meeseeks[player][index_meeseek]
extern uint8_t init_tasks[PLAYERS];
extern uint32_t cr3[PLAYERS];

void tss_creator(int8_t player, int task);
paddr_t tss_meeseeks_creator(player_t player, uint8_t task, uint32_t code_start, coordenadas coord);

#endif //  __TSS_H__
