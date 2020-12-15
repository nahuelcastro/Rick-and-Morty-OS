/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"

player_t ultimoJugador;  // 0001 0011
uint16_t jugadorActual = 1;
uint16_t index;
uint16_t tareaActual;
tss_t* tssActual;
uint16_t tareaActualAnterior;
uint8_t clocks[PLAYERS][MAX_CANT_MEESEEKS];
const char* CLOCK[4] = {"| ", "/ ", "- ", "\\ "};

extern void pantalla_negra_debug();
extern void init_pantalla();

sched_t sched[PLAYERS][11];

bool modoDebug;

void sched_init(void)
{
  index = 16;
  tareaActual = index;
  tareaActualAnterior = tareaActual;
  modoDebug = false;

  for (player_t player = MORTY; player < PLAYERS ; player++){
    for (int i = 0; i < 11; i++){
      sched[player][i].p_loop_sched = 0;
      sched[player][i].info_task->flag_loop = 0;
      sched[player][i].info_task->active = false;
    }
    for (int i = 0; i < MAX_CANT_MEESEEKS; i++){
      clocks[player][i] = 0;
    }
  }
}



void reset_p_loop_task(player_t player){
  for (uint8_t i = 0; i < 11; i++){
    if(sched[player][i].info_task->active){
      sched[player][i].info_task->flag_loop = false;
    }
  }
}



info_task_t* next(player_t player){

  //   print("t:",1,41,WHITE_BLACK);
  // print_dec(tareaActual,1 , 3 ,41,WHITE_BLACK);

  // print("p:",1, 43,WHITE_BLACK);
  // print_dec(player,1 , 3 ,43,WHITE_BLACK);

  for (uint8_t i = 0; i < 11; i++){
    // bool p_loop_sched = /*sched[player][i].p_loop_sched*/ false;
    bool active = sched[player][i].info_task->active;
    bool p_loop_task = sched[player][i].info_task->flag_loop;

    if( active && !p_loop_task ){
        sched[player][i].info_task->flag_loop = true;
        sched[player][i].p_loop_sched = true;
        // print_dec(sched[player][i].info_task->idx_gdt,4, 70,30,WHITE_RED); 
        return sched[player][i].info_task;
    }
  }

  reset_p_loop_task(player);

  for (uint8_t i = 0; i < 11; i++){
    bool active = sched[player][i].info_task->active;
    bool p_loop_task = sched[player][i].info_task->flag_loop;
    if( active && !p_loop_task ){
        sched[player][i].info_task->flag_loop = true;
        sched[player][i].p_loop_sched = true;
        return sched[player][i].info_task;
    }
  }

  print( "LLEGO HASTA ACA BRO, ANDA TODO MAL :(", 1,2,WHITE_RED); // dejarlo porque sirve de advertencia
  breakpoint();
  //! ACA NO TIENE QUE LLEGAR, POR ENDE, SI LLEGA ACA HAY QUE LLAMAR A ENDGAME
  info_task_t* task_que_nunca_va_llegar = sched[player][0].info_task; // pero al compilar me jode que ponga un return aca
  return task_que_nunca_va_llegar;
}

bool end_loop_sched(){
  bool ret = true;
  for (player_t player = 0; player < PLAYERS; player++){
    for (uint8_t i = 0; i < 11; i++){
      if(sched[player][i].info_task->active){        
        ret = ret && sched[player][i].p_loop_sched; 
      }
    }
  }
  return ret;
}

void reset_sched_p(){
  for (player_t player = 0; player < PLAYERS; player++){
    for (size_t i = 0; i < 11; i++){
      sched[player][i].p_loop_sched = false;
    }
  }
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


// //! SCHED NAJU
uint16_t sched_next_task(void){

  player_t new_player;
  tareaActual = tareaActualAnterior;

  if(tareaActualAnterior == IDLE){ 
    tareaActual++;
    new_player = RICK; // ver si 
  }else{
    new_player = info_task[tareaActual].player ? MORTY : RICK;
  }

  info_task_t* task = next(new_player);
  index = task->idx_gdt;

  bool msk_task = index > 18 ;
  if (msk_task){
    update_msk_clocks(task);                   
  }

  bool end_loop= end_loop_sched(); 
  if(end_loop){
    reset_sched_p();
  }

  tareaActual = index;
  tareaActualAnterior = index;

  return (task->idx_gdt << 3);

}
    


void desactivar_tarea(){
  if(tareaActual == 17 || tareaActual == 18){
    end_game();
  }
  //tareasActivas[tareaActual] = false;
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
  tareaActual = 16;;
  return 0x80;
}


int proximoStack(int i){
  // Agarrar el esp del tssActual y devolver la proxima instrucción
  return i;
}

int codigoError(void){
  // Buscar la x posicion en la pila para devolver el err tssActual-> esp + 
  return 0;
}

uint32_t registros[15];
char registrosNombre[15][10] = {"eax", "ebx","ecx","edx","esi","edi","ebp","esp","eip","cs","ds","es","fs","gs","ss"};

//register uint8_t valor_scancode asm("al");

void modo_debug(void){

  if(modoDebug){

    modoDebug=false;
    init_pantalla();

  } else{

    modoDebug=true;
    pantalla_negra_debug();
    registros[0]  = tssActual->eax;
    registros[1]  = tssActual->ebx;
    registros[2]  = tssActual->ecx;
    registros[3]  = tssActual->edx;
    registros[4]  = tssActual->esi;
    registros[5]  = tssActual->edi;
    registros[6]  = tssActual->ebp;
    registros[7]  = tssActual->esp;
    registros[8]  = tssActual->eip;
    registros[9]  = tssActual->cs;
    registros[10] = tssActual->ds;
    registros[11] = tssActual->es;
    registros[12] = tssActual->fs;
    registros[13] = tssActual->gs;
    registros[14] = tssActual->ss;

    if(ultExcepcion!=260){
      imprimir_excepcion(ultExcepcion);
    }
    for(int i=0; i<30; i = i + 2){
      print((char*)(registrosNombre + (i/2)), 21, i+4, C_FG_WHITE);
      print_hex(registros[i/2],8, 25, i+4, C_FG_LIGHT_GREEN);
    }
    print("eflags",21,37,C_FG_WHITE);
    print_hex(tssActual->eflags,8, 28, 37, C_FG_LIGHT_GREEN);

    print("stack", 36, 17, C_FG_WHITE);
    for(int i=0; i<6; i = i + 2){
      print_hex(proximoStack(i/2), 8, 36, i+19, C_FG_LIGHT_GREEN);
    }
    
    print("backtrace", 36, 25, C_FG_WHITE);
    print("00000000" , 36, 27, C_FG_LIGHT_GREEN);
    print("00000000" , 36, 29, C_FG_LIGHT_GREEN);
    print("00000000" , 36, 31, C_FG_LIGHT_GREEN);
    print("00000000" , 36, 33, C_FG_LIGHT_GREEN);

    print("cr0", 45, 5, C_FG_WHITE);
    print_hex(rcr0(), 8, 49, 5, C_FG_LIGHT_GREEN);

    print("cr2", 45, 7, C_FG_WHITE);
    print_hex(rcr2(), 8, 49, 7, C_FG_LIGHT_GREEN);

    print("cr3", 45, 9, C_FG_WHITE);
    print_hex(rcr3(),8 , 49, 9, C_FG_LIGHT_GREEN);

    print("cr4", 45, 11, C_FG_WHITE);
    print_hex(rcr4(),8 , 49, 11, C_FG_LIGHT_GREEN);


    print("err", 45, 13, C_FG_WHITE);
    print_hex(codigoError(),8 , 49, 13, C_FG_LIGHT_GREEN);
  }
  
}


/*

  ! Rompe esquemas, ver que onda en casos extremos como este :)
  1) Rick
  2) Morty

  3)Rick_m
  4)Morty_m 
  5)Morty_m
  6)Morty_m
  7)Morty_m
  8)Morty_m
  9)Morty_m
  10)Morty_m
  11)Morty_m
  12)Morty_m

1 2 3 4 1 5 2 6

r m r m r m r m r m
1 2 3 4 1 5 3 6 1 4

ciclo entero
rick[1,3]
morty[2,4,6,7,8...]
14
contador_de_una_vez = 15 entonces reinicia todo

sched[player].dame  1 
sched[player].dame  2
sched[player].dame  3
sched[player].dame  4
sched[player].dame  1
sched[player].dame  
sched[player].dame



    ! printear info importante 
   
    //! MAXI NO BORRES COMENTARIOS QUE SON LA PUTA HOSTIA (PRINTEADOR DE TAREAS)
    for (int i = 0; i < 41; i++){
      print_dec(9 ,16 , 5,i,GREEN_GREEN);
    }

    print("i" , 5 ,4,WHITE_RED);          // i
    print("a" , 8 ,4,WHITE_RED);          // task_active
    print("Pl", 10,4,WHITE_RED);          // p_loop_sched
    print("Fl", 13,4,WHITE_RED);          // flag_loop
    print("ig", 16,4,WHITE_RED);          // idx_gdt
    print("ma", 19,4,WHITE_RED);          // meeseek activo
    print("player :", 17,0,WHITE_RED);
    print_dec(player,1 , 25,0,WHITE_RED);


    for (uint8_t i = 0; i < 11 ; i++){
      int j = 6 + 2*i;
      print_dec(i + 1 ,2 , 5,j,WHITE_RED);
      print_dec(sched[player][i].info_task->active,1, 8, j,WHITE_RED);
      print_dec(sched[player][i].p_loop_sched,1, 10, j,WHITE_RED);
      print_dec(sched[player][i].info_task->flag_loop,1, 13,j,WHITE_RED);
      print_dec(sched[player][i].info_task->idx_gdt,2, 16,j,WHITE_RED);
      if( i > 0 ){
        print_dec(meeseeks[player][i-1].p,1, 19,j,WHITE_RED);
      }
    }




*/