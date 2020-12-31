/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"
extern void pantalla_negra_debug();
extern void init_pantalla();
extern void registrosActuales();

player_t ultimoJugador;  // 0001 0011
uint16_t jugadorActual = 1;
uint16_t tareaActual;
uint16_t tareaActualAnterior;

const char* CLOCK[4] = {"| ", "/ ", "- ", "\\ "};

bool modoDebug;
bool exception;
bool returning_debug_mode;
bool debug_executing;

sched_t sched[PLAYERS][11];
uint32_t backup_map[80*41];


void sched_init(void){
  tareaActual = 16;
  tareaActualAnterior = tareaActual;
  modoDebug = true;

  for (player_t player = MORTY; player < PLAYERS ; player++){
    for (int i = 0; i < 11; i++){
      sched[player][i].info_task->flag_loop = 0;
      sched[player][i].info_task->active = false;
    }
  }
  returning_debug_mode = false;
}



void reset_p_loop_task(player_t player){
  for (uint8_t i = 0; i < 11; i++){
    if(sched[player][i].info_task->active){
      sched[player][i].info_task->flag_loop = false;
    }
  }
}



info_task_t* next(player_t player){

  for (uint8_t i = 0; i < 11; i++){
    bool active = sched[player][i].info_task->active;
    bool p_loop_task = sched[player][i].info_task->flag_loop;

    if( active && !p_loop_task ){
        sched[player][i].info_task->flag_loop = true;
        return sched[player][i].info_task;
    }
  }

  reset_p_loop_task(player);

  for (uint8_t i = 0; i < 11; i++){
    bool active = sched[player][i].info_task->active;
    bool p_loop_task = sched[player][i].info_task->flag_loop;
    if( active && !p_loop_task ){
        sched[player][i].info_task->flag_loop = true;
        return sched[player][i].info_task;
    }
  }

  breakpoint();
  //! ACA NO TIENE QUE LLEGAR, POR ENDE, SI LLEGA ACA HAY QUE LLAMAR A ENDGAME
  end_game();
  info_task_t* task_que_nunca_va_llegar = sched[player][0].info_task; // pero al compilar me jode que ponga un return aca
  return task_que_nunca_va_llegar;
}



void update_msk_clocks(info_task_t* task){
  task->clock = (task->clock + 1) %4;
  uint8_t i = task->clock;
  coordenadas coord; 
  coord.x = 26 + 3 * task->idx_msk;
  coord.y = 48 - 4 * task->player;
  print("C", coord.x, coord.y, BLACK_BLACK);
  print(CLOCK[i], coord.x, coord.y, WHITE_BLACK);
}


uint16_t sched_next_task(void){

  if (modoDebug){
    print("Debug on",1,48, WHITE_BLACK);
  } else{
    print("Debug on",1,48, BLACK_BLACK);
  }
  

  if (modoDebug && debug_executing){
    return (IDLE << 3);
  }

  player_t new_player;
  tareaActual = tareaActualAnterior;

  if(tareaActualAnterior == IDLE){
    new_player = RICK;
  }else{
    new_player = info_task[tareaActual].player ? MORTY : RICK;
  }

  info_task_t* task = next(new_player);
  tareaActual = task->idx_gdt;
  tareaActualAnterior = task->idx_gdt;

  bool msk_task = tareaActual > 18 ;
  if (msk_task){
    update_msk_clocks(task);                   
  }

  return (task->idx_gdt << 3);

}
  


void desactivar_tarea(){
  
  if(tareaActual < 0x13 || tareaActual > 0x26 ){
    end_game();
  }

  info_task[tareaActual].active = false;
  info_task[tareaActual].clock = 0;
  coordenadas coord; 
  coord.x = 26 + 3 * info_task[tareaActual].idx_msk;
  coord.y = 48 - 4 * info_task[tareaActual].player;
  print("C ", coord.x, coord.y, BLACK_BLACK);
  print("X ", coord.x, coord.y, WHITE_BLACK);
}

uint16_t sched_idle(){
  tareaActualAnterior = tareaActual;
  tareaActual = 16;
  return 0x80;
}

void desactivar_tarea_anterior(){
  
  if(tareaActual < 0x13 || tareaActualAnterior > 0x26 ){
    end_game();
  }

  info_task[tareaActualAnterior].active = false;
  info_task[tareaActualAnterior].clock = 0;
  coordenadas coord; 
  coord.x = 26 + 3 * info_task[tareaActualAnterior].idx_msk;
  coord.y = 48 - 4 * info_task[tareaActualAnterior].player;
  print("C ", coord.x, coord.y, BLACK_BLACK);
  print("X ", coord.x, coord.y, WHITE_BLACK);
}


void imprimirRegistros(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi, uint32_t ebp,/*uint32_t esp,*/ uint32_t eip, uint16_t cs, uint16_t ds, uint16_t es, uint16_t fs, uint16_t gs, uint16_t ss, uint16_t eflags ,uint16_t except_code, uint32_t error_code,uint32_t esp){
  
  modo_debug();
  imprimir_excepcion(except_code);
  
  esp = esp + 6*4; // para evitar todas las cosas que se pushean al entrar a la interrupción (cuadno hay cambio de nivel de privilegio)

  print_hex(tareaActual, 2, 56,2, C_FG_LIGHT_GREEN);

  int y = 6;

  print("EAX:",21,y,C_FG_WHITE);  
  print_hex(eax,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("EBX:",21,y,C_FG_WHITE);
  print_hex(ebx,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("ECX:",21,y,C_FG_WHITE);
  print_hex(ecx,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("EDX:",21,y,C_FG_WHITE);
  print_hex(edx,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("ESI:",21,y,C_FG_WHITE);
  print_hex(esi,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("EDI:",21,y,C_FG_WHITE);
  print_hex(edi,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("EBP:",21,y,C_FG_WHITE);
  print_hex(ebp,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("ESP:",21,y,C_FG_WHITE);
  print_hex(esp,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("EIP:",21,y,C_FG_WHITE);
  print_hex(eip,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("CS:",21,y,C_FG_WHITE);
  print_hex(cs,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("DS:",21,y,C_FG_WHITE);
  print_hex(ds,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("ES:",21,y,C_FG_WHITE);
  print_hex(es,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("FS:",21,y,C_FG_WHITE);
  print_hex(fs,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("GS:",21,y,C_FG_WHITE);
  print_hex(gs,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("SS:",21,y,C_FG_WHITE);
  print_hex(ss,8,25,y,C_FG_LIGHT_GREEN);
  y += 2;

  print("EFLAGS:",21,y,C_FG_WHITE);
  print_hex(eflags,8,28,y,C_FG_LIGHT_GREEN);
  y += 2;


  print("err", 45, 14, C_FG_WHITE);
  print_hex(error_code,8 , 49, 14, C_FG_LIGHT_GREEN);


  print("stack", 36, 17, C_FG_WHITE);
  uint32_t * stack = (uint32_t * )(esp);  // estaba 18 * 4
  y = 19;
  for(int a = 0; a < 3; a++){
      print_hex(*stack,8,36,y, C_FG_LIGHT_GREEN);
      ++stack;
      y += 2;
  }



}



void debug_mode_on_off(){
  modoDebug = !modoDebug;

  if (debug_executing && !modoDebug ){

      desactivar_tarea_anterior();

      debug_executing = false;
      modoDebug = true;
      print("Debug on",1,48, BLACK_BLACK);
    
      //restaurar pantalla
      uint32_t* video = (uint32_t*) VIDEO;
      for(uint32_t i = 0; i< 80*41; i++) {
        video[i] = backup_map[i];
    }
  }
    

}

void modo_debug(){
  
  if(modoDebug){

    debug_executing = true;
    
    
    uint32_t* video = (uint32_t*) VIDEO;
    for(uint32_t i = 0; i< 80*41; i++) {
      backup_map[i] = video[i];
    }

    pantalla_negra_debug();


    if(ultExcepcion!=260){
      imprimir_excepcion(ultExcepcion);
    }
   print("backtrace", 36, 25, C_FG_WHITE);
    for (size_t i = 0; i < 4; i++)
    {
      print_hex(0, 8, 36, (27 + 2*i), C_FG_LIGHT_GREEN);
    }


    print("cr0", 45, 6, C_FG_WHITE);
    print_hex(rcr0(), 8, 49, 6, C_FG_LIGHT_GREEN);

    print("cr2", 45, 8, C_FG_WHITE);
    print_hex(rcr2(), 8, 49, 8, C_FG_LIGHT_GREEN);

    print("cr3", 45, 10, C_FG_WHITE);
    print_hex(rcr3(),8 , 49, 10, C_FG_LIGHT_GREEN);

    print("cr4", 45, 12, C_FG_WHITE);
    print_hex(rcr4(),8 , 49, 12, C_FG_LIGHT_GREEN);

  }

}
