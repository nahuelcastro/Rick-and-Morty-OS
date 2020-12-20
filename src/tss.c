/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"
#include "i386.h"
#include "sched.h"


#define IDX_TSS_INIT 15
#define IDX_TSS_IDLE 16
#define IDLE_TASK_CODE 0x18000 //
#define IDX_CODE_LVL_0 0x50
#define IDX_DATO_LVL_0 0x58    //0101 1000 58 
#define IDX_CODE_LVL_3 0x63    //0110 0000  12   ahora ->    0110 0011
#define IDX_DATO_LVL_3 0x6b    //0110 1000  13   ahora ->    0110 1011
#define PAGE_SIZE 4096
#define TASK_VIRTUAL_DIR 0x1D00000

tss_t tss_initial;
tss_t tss_idle;

tss_t tss_rick[11];   // 10
tss_t tss_morty[11];  // 10

tss_t* TSSs[35];
uint8_t init_tasks[PLAYERS];

meeseek_t meeseeks[PLAYERS][MAX_CANT_MEESEEKS];
uint8_t cant_meeseeks[PLAYERS];

uint32_t cr3[PLAYERS];

uint32_t gdt_index = 0;

info_task_t info_task[GDT_COUNT];


void init_tss(void) {


  for (size_t i = 0; i < 35; i++){
    //player_idx_gdt[i] = -1;
    info_task[i].player = NULL_PLAYER;


  }
  
  for (size_t i = 0; i < GDT_COUNT; i++){
    info_task[i].active = false;
  }
  
  uint32_t base = (uint32_t) &tss_initial;
  tss_gdt_entry_init(IDX_TSS_INIT, base, 0);

  TSSs[15] = &tss_initial;

  cant_meeseeks[MORTY] = 0;
  cant_meeseeks[RICK] = 0;

  init_tasks[MORTY] = 0;
  init_tasks[RICK] = 0;

  gdt_index = 0;
  
}

void init_idle(){

  uint32_t base_idle_task = (uint32_t) &tss_idle;
  tss_gdt_entry_init(IDX_TSS_IDLE, base_idle_task, 0);

  tss_idle.ptl = 0;
  tss_idle.unused0 = 0;
  tss_idle.esp0 = 0; //KERNEL_STACK;    marco lo dijo asi 
  tss_idle.ss0 = 0; //IDX_DATO_LVL_0;
  tss_idle.unused1 = 0;
  tss_idle.esp1 = 0;
  tss_idle.ss1 = 0;
  tss_idle.unused2 = 0;
  tss_idle.esp2 = 0;
  tss_idle.ss2 = 0;
  tss_idle.unused3 = 0;
  tss_idle.cr3 = KERNEL_PAGE_DIR;
  tss_idle.eip = IDLE_TASK_CODE;
  tss_idle.eflags = 0x202;
  tss_idle.eax = 0;
  tss_idle.ecx = 0;
  tss_idle.edx = 0;
  tss_idle.ebx = 0;
  tss_idle.esp = KERNEL_STACK;
  tss_idle.ebp = 0;
  tss_idle.esi = 0;
  tss_idle.edi = 0;
  tss_idle.es = IDX_DATO_LVL_0;
  tss_idle.unused4 = 0;
  tss_idle.cs = IDX_CODE_LVL_0;
  tss_idle.unused5 = 0;
  tss_idle.ss = IDX_DATO_LVL_0;
  tss_idle.unused6 = 0;
  tss_idle.ds = IDX_DATO_LVL_0;
  tss_idle.unused7 = 0;
  tss_idle.fs = IDX_DATO_LVL_0;
  tss_idle.unused8 = 0;
  tss_idle.gs = IDX_DATO_LVL_0;
  tss_idle.unused9 = 0;
  tss_idle.ldt = 0;
  tss_idle.unused10 = 0;
  tss_idle.dtrap = 0;
  tss_idle.iomap = 0xFFFF;

  TSSs[16] = &tss_idle;
}


void tss_gdt_entry_init(uint32_t index, uint32_t tss, int dpl) {

    gdt[index].limit_15_0 = 0x0067;
    gdt[index].base_15_0 = tss & 0xFFFF;
    gdt[index].base_23_16 = (tss >> 16) & 0xFF;
    gdt[index].base_31_24 = (tss >> 24);
    gdt[index].type = 0x9;
    gdt[index].s = 0x0;
    gdt[index].dpl = dpl;
    gdt[index].p = 0x1;
    gdt[index].limit_19_16 = 0x0;
    gdt[index].avl = 0x1;
    gdt[index].l = 0x0;
    gdt[index].db = 0x0;
    gdt[index].g = 0x0;

}


uint32_t next_free_gdt_idx = 16;

void next_free_tss() {    
  next_free_gdt_idx++;  
}


void tss_creator(int8_t player, int task){

  paddr_t code_start;
  paddr_t task_phy_address;
  tss_t* tss_new_task;
  if(player == RICK){                                   // 1 = Rick
    task_phy_address = 0x1D00000;
    code_start = 0x10000;
    tss_new_task = &tss_rick[task];
  }else{                                             // 0 = Morty
    task_phy_address = 0x1D04000;
    code_start = 0x14000;
    tss_new_task = &tss_morty[task];
  }
  paddr_t new_cr3 = mmu_init_task_dir(task_phy_address, code_start,4);
  paddr_t stack_level_0 = mmu_next_free_kernel_page();

  cr3[player] = new_cr3;

  next_free_tss();
  
  info_task[next_free_gdt_idx].player = player;
  info_task[next_free_gdt_idx].active = true;
  info_task[next_free_gdt_idx].flag_loop = 0;
  info_task[next_free_gdt_idx].idx_gdt = next_free_gdt_idx;
  

  sched[player][0].p_loop_sched = 0; 
  sched[player][0].info_task = &info_task[next_free_gdt_idx]; 

  init_tasks[player] ++;
  

  tss_gdt_entry_init(next_free_gdt_idx, (uint32_t) tss_new_task, 3);

  tss_new_task->ptl = 0; //(uint32_t) tss_new_task;
  tss_new_task->unused0 = 0;
  tss_new_task->esp0 = stack_level_0 + PAGE_SIZE;
  tss_new_task->ss0 = IDX_DATO_LVL_0;
  tss_new_task->unused1 = 0;
  tss_new_task->esp1 = 0;
  tss_new_task->ss1 = 0;
  tss_new_task->unused2 = 0;
  tss_new_task->esp2 = 0;
  tss_new_task->ss2 = 0;
  tss_new_task->unused3 = 0;
  tss_new_task->cr3 = new_cr3;
  tss_new_task->eip = TASK_VIRTUAL_DIR;
  tss_new_task->eflags = 0x202;
  tss_new_task->eax = 0;
  tss_new_task->ecx = 0;
  tss_new_task->edx = 0;
  tss_new_task->ebx = 0;
  tss_new_task->esp = TASK_VIRTUAL_DIR + 4 * PAGE_SIZE;
  tss_new_task->ebp = 0; 
  tss_new_task->esi = 0;
  tss_new_task->edi = 0;
  tss_new_task->es  = IDX_DATO_LVL_3;
  tss_new_task->unused4 = 0;
  tss_new_task->cs = IDX_CODE_LVL_3;
  tss_new_task->unused5 = 0;
  tss_new_task->ss = IDX_DATO_LVL_3;
  tss_new_task->unused6 = 0;
  tss_new_task->ds = IDX_DATO_LVL_3;
  tss_new_task->unused7 = 0;
  tss_new_task->fs = IDX_DATO_LVL_3;
  tss_new_task->unused8 = 0;
  tss_new_task->gs = IDX_DATO_LVL_3;
  tss_new_task->unused9 = 0;
  tss_new_task->ldt = 0;
  tss_new_task->unused10 = 0;
  tss_new_task->dtrap = 0;
  tss_new_task->iomap = 0xFFFF;

  if (player == 1){                                 // 1 = Rick
    TSSs[next_free_gdt_idx] = tss_new_task;
  }
  else{                                             // 0 = Morty
    TSSs[next_free_gdt_idx] = tss_new_task;
  }
}



paddr_t tss_meeseeks_creator(player_t player,uint8_t task, uint32_t code_start, coordenadas coord){

  
  uint32_t cr3 = rcr3();    

  // paddr_t code_start;
  paddr_t task_phy_address;
  paddr_t task_virt_address;
  tss_t *tss_new_task;

  if (player == RICK){
    tss_new_task = &tss_rick[task];
  }else{
    tss_new_task = &tss_morty[task];
  }

  int8_t idx_msk = task - 1;

  // consigo proxima phy, virt y stack_lvl_0 libres para meeseeks
  paddr_t stack_level_0;
  task_phy_address = mmu_phy_map_decoder(coord);
  bool reciclar = backup_meeseks[player][idx_msk].p;

  if (reciclar){

    task_virt_address = backup_meeseks[player][idx_msk].virt;
    mmu_init_task_meeseeks_dir(task_phy_address, code_start, task_virt_address);
    gdt_index = backup_meeseks[player][idx_msk].gdt_index;
    stack_level_0 = backup_meeseks[player][idx_msk].stack_level_0;
  } else{
    task_virt_address = mmu_next_free_virt_meeseek_page(player);
    mmu_init_task_meeseeks_dir(task_phy_address, code_start, task_virt_address);
    next_free_tss();

    char* stack0 = (char*) backup_meeseks[player][idx_msk].stack_level_0;
    for (size_t i = 0; i < 4096; i++){
      stack0 = 0;
      stack0++;
    }
    backup_meeseks[player][idx_msk].stack_level_0 = (paddr_t) stack0;

    gdt_index = next_free_gdt_idx;
    stack_level_0 = mmu_next_free_kernel_page();    //! VER QUE CUANDO LO QUIERO LIMPIAR ROMPE
     
    // guardo la info importante para luego poder reciclar la memoria de meeseeks muertos
    backup_meeseks[player][idx_msk].p    = true;
    backup_meeseks[player][idx_msk].virt = task_virt_address;
    backup_meeseks[player][idx_msk].gdt_index = gdt_index;
    backup_meeseks[player][idx_msk].stack_level_0 = stack_level_0;   //! VER QUE CUANDO LO QUIERO LIMPIAR ROMPE
  }

  

  //stack_level_0 = mmu_next_free_kernel_page();    /* //! DESPUES HAY QUE SACARLO*/ 

  info_gdt_meeseeks[gdt_index].idx_msk = idx_msk;
  info_gdt_meeseeks[gdt_index].player = player;
  info_gdt_meeseeks[gdt_index].ticks_counter = 0;


  info_task[gdt_index].player = player;
  info_task[gdt_index].active = true;
  info_task[gdt_index].flag_loop = 0;
  info_task[gdt_index].idx_gdt = gdt_index;
  info_task[gdt_index].idx_msk = idx_msk;
  info_task[gdt_index].clock = 0;

  meeseeks[player][idx_msk].gdt_index = gdt_index;

  sched[player][idx_msk + 1].p_loop_sched = 0; 
  sched[player][idx_msk + 1].info_task = &info_task[gdt_index]; 

  init_tasks[player] ++;

  if(reciclar){
    // poner los valores en la TSS vieja

    tss_t *tss = backup_meeseks[player][idx_msk].tss;

    tss->ptl = 0; //(uint32_t) tss_new_task;
    tss->unused0 = 0;
    tss->esp0 = stack_level_0 + PAGE_SIZE;
    tss->ss0 = IDX_DATO_LVL_0;
    tss->unused1 = 0;
    tss->esp1 = 0;
    tss->ss1 = 0;
    tss->unused2 = 0;
    tss->esp2 = 0;
    tss->ss2 = 0;
    tss->unused3 = 0;
    tss->cr3 = cr3;
    tss->eip = task_virt_address;
    tss->eflags = 0x202;
    tss->eax = 0;
    tss->ecx = 0;
    tss->edx = 0;
    tss->ebx = 0;
    tss->esp = task_virt_address + 2 * PAGE_SIZE;     
    tss->ebp = 0; //TASK_VIRTUAL_DIR + 4 * PAGE_SIZE;
    tss->esi = 0;
    tss->edi = 0;
    tss->es = IDX_DATO_LVL_3;
    tss->unused4 = 0;
    tss->cs = IDX_CODE_LVL_3;
    tss->unused5 = 0;
    tss->ss = IDX_DATO_LVL_3;
    tss->unused6 = 0;
    tss->ds = IDX_DATO_LVL_3;
    tss->unused7 = 0;
    tss->fs = IDX_DATO_LVL_3;
    tss->unused8 = 0;
    tss->gs = IDX_DATO_LVL_3;
    tss->unused9 = 0;
    tss->ldt = 0;
    tss->unused10 = 0;
    tss->dtrap = 0;
    tss->iomap = 0xFFFF;



  } else{
    tss_gdt_entry_init(gdt_index, (uint32_t)tss_new_task, 3);

    tss_new_task->ptl = 0; //(uint32_t) tss_new_task;
    tss_new_task->unused0 = 0;
    tss_new_task->esp0 = stack_level_0 + PAGE_SIZE;
    tss_new_task->ss0 = IDX_DATO_LVL_0;
    tss_new_task->unused1 = 0;
    tss_new_task->esp1 = 0;
    tss_new_task->ss1 = 0;
    tss_new_task->unused2 = 0;
    tss_new_task->esp2 = 0;
    tss_new_task->ss2 = 0;
    tss_new_task->unused3 = 0;
    tss_new_task->cr3 = cr3;
    tss_new_task->eip = task_virt_address;
    tss_new_task->eflags = 0x202;
    tss_new_task->eax = 0;
    tss_new_task->ecx = 0;
    tss_new_task->edx = 0;
    tss_new_task->ebx = 0;
    tss_new_task->esp = task_virt_address + 2 * PAGE_SIZE;     
    tss_new_task->ebp = 0; //TASK_VIRTUAL_DIR + 4 * PAGE_SIZE;
    tss_new_task->esi = 0;
    tss_new_task->edi = 0;
    tss_new_task->es = IDX_DATO_LVL_3;
    tss_new_task->unused4 = 0;
    tss_new_task->cs = IDX_CODE_LVL_3;
    tss_new_task->unused5 = 0;
    tss_new_task->ss = IDX_DATO_LVL_3;
    tss_new_task->unused6 = 0;
    tss_new_task->ds = IDX_DATO_LVL_3;
    tss_new_task->unused7 = 0;
    tss_new_task->fs = IDX_DATO_LVL_3;
    tss_new_task->unused8 = 0;
    tss_new_task->gs = IDX_DATO_LVL_3;
    tss_new_task->unused9 = 0;
    tss_new_task->ldt = 0;
    tss_new_task->unused10 = 0;
    tss_new_task->dtrap = 0;
    tss_new_task->iomap = 0xFFFF;

    backup_meeseks[player][idx_msk].tss = tss_new_task;

  }


  if (player == RICK){
    TSSs[gdt_index] = tss_new_task;
  }
  else{
    TSSs[gdt_index] = tss_new_task;
  }

  return task_virt_address;
}





/*

<bochs:313> info tss
tr:s=0x90, base=0x00008e20, valid=1
ss:esp(0): 0x0058:0x00008e88
ss:esp(1): 0x0000:0x00000000
ss:esp(2): 0x0000:0x00000011
cr3: 0x00000000
eip: 0x00008e54
eflags: 0x0000208c
cs: 0x0000 ds: 0x0013 ss: 0x003b
es: 0x0000 fs: 0x0063 gs: 0x0246
eax: 0x01d0003b  ebx: 0x00000000  ecx: 0x00000011  edx: 0x00002099
esi: 0x00008e74  edi: 0x00000011  ebp: 0x00000000  esp: 0x00000000
ldt: 0x3ffc
i/o map: 0x0000
<bochs:314> 


<bochs:217> info tss
tr:s=0x90, base=0x00008e20, valid=1
ss:esp(0): 0x0058:0x00108000
ss:esp(1): 0x0000:0x00000000
ss:esp(2): 0x0000:0x00000000
cr3: 0x00104000
eip: 0x000020a5
eflags: 0x00000006
cs: 0x0050 ds: 0x006b ss: 0x0058
es: 0x006b fs: 0x006b gs: 0x006b
eax: 0x00000080  ebx: 0x0000000c  ecx: 0x00000000  edx: 0x00000000
esi: 0x00000000  edi: 0x00000000  ebp: 0x00107fcc  esp: 0x00107fcc
ldt: 0x0000
i/o map: 0xffff
<bochs:218> 











*/