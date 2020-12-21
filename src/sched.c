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
// bool exception =0;
tss_t* tssActual;
uint16_t tareaActualAnterior;
uint8_t clocks[PLAYERS][MAX_CANT_MEESEEKS];
const char* CLOCK[4] = {"| ", "/ ", "- ", "\\ "};

extern void pantalla_negra_debug();
extern void init_pantalla();
extern void registrosActuales();

sched_t sched[PLAYERS][11];

bool modoDebug;
bool exception;
bool returning_debug_mode;
bool debug_executing;

uint32_t backup_map[80*41];


void sched_init(void){
  index = 16;
  tssActual = TSSs[16];
  tareaActual = index;
  tareaActualAnterior = tareaActual;
  modoDebug = true;

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
        sched[player][i].p_loop_sched = true;

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
  coord.y = 49 - 4 * task->player;
  print("C", coord.x, coord.y, BLACK_BLACK);
  print(CLOCK[i], coord.x, coord.y, WHITE_BLACK);
}


uint16_t sched_next_task(void){

  if (modoDebug){
    print("D",1,48, WHITE_BLACK);
  }

  if (modoDebug && exception){
    tssActual = TSSs[IDLE];
    return (IDLE << 3);
  }

  player_t new_player;
  tareaActual = tareaActualAnterior;



  if(tareaActualAnterior == IDLE){ 
    tareaActual++;
    new_player = RICK; // ver si 
  }else{
    new_player = info_task[tareaActual].player ? MORTY : RICK;
  }



    //   //! MAXI NO BORRES COMENTARIOS QUE SON LA PUTA HOSTIA (PRINTEADOR DE TAREAS)
    // for (int i = 0; i < 41; i++){
    //   print_dec(9 ,16 , 5,i,GREEN_GREEN);
    // }
    // print("i" , 5 ,4,WHITE_RED);          // i
    // print("a" , 8 ,4,WHITE_RED);          // task_active
    // print("Pl", 10,4,WHITE_RED);          // p_loop_sched
    // print("Fl", 13,4,WHITE_RED);          // flag_loop
    // print("ig", 16,4,WHITE_RED);          // idx_gdt
    // print("ma", 19,4,WHITE_RED);          // meeseek activo
    // print("player :", 17,0,WHITE_RED);
    // print_dec(new_player,1 , 25,0,WHITE_RED);
    // for (uint8_t i = 0; i < 11 ; i++){
    //   int j = 6 + 2*i;
    //   print_dec(i + 1 ,2 , 5,j,WHITE_RED);
    //   print_dec(sched[new_player][i].info_task->active,1, 8, j,WHITE_RED);
    //   print_dec(sched[new_player][i].p_loop_sched,1, 10, j,WHITE_RED);
    //   print_dec(sched[new_player][i].info_task->flag_loop,1, 13,j,WHITE_RED);
    //   print_dec(sched[new_player][i].info_task->idx_gdt,2, 16,j,WHITE_RED);
    //   if( i > 0 ){
    //     print_dec(meeseeks[new_player][i-1].p,1, 19,j,WHITE_RED);
    //   }
    // }

    // // breakpoint();






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

  tssActual = TSSs[task->idx_gdt];
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
  coord.y = 49 - 4 * info_task[tareaActual].player;
  print("C ", coord.x, coord.y, BLACK_BLACK);
  print("X ", coord.x, coord.y, WHITE_BLACK);


}

uint16_t sched_idle(){
  tareaActualAnterior = tareaActual;
  tareaActual = 16;
  return 0x80;
}


uint32_t proximoStack(int i){
  // Agarrar el esp del tssActual y devolver la proxima instrucción
  char* stack = (char*) tssActual->esp + (i-1);
  return (uint32_t) stack;
}

int codigoError(void){
  // Buscar la x posicion en la pila para devolver el err tssActual-> esp + 
  return 0;
}

//uint32_t registros[15];

//register uint8_t valor_scancode asm("al");

//16 parametros
void imprimirRegistros(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi, uint32_t ebp, uint32_t esp, uint32_t eip, uint32_t cs, uint32_t ds, uint32_t es, uint32_t fs, uint32_t gs, uint32_t ss, uint32_t eflags ,uint32_t except_code){
  // char registrosNombre[16][10] = {"eax", "ebx","ecx","edx","esi","edi","ebp","esp","eip","cs","ds","es","fs","gs","ss","eflags"};
  // uint32_t registros[16] = {eax,ebx,ecx,edx,esi,edi,ebp,esp,eip,cs,ds,es,fs,gs,ss,eflags};
  // for (size_t i = 0; i < 30; i+=2){
  //   print((char*)(registrosNombre + (i/2)), 21, i+4, C_FG_WHITE);
  //   print_hex(registros[i/2],8, 25, i+4, C_FG_LIGHT_GREEN);
  // }

  
  modo_debug();
  imprimir_excepcion(except_code);
  

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


  // y = 19;
  // print_hex(stack_p0,8,36,y,C_FG_LIGHT_GREEN);
  // y += 2;
  
  // print_hex(eip,8,36,y,C_FG_LIGHT_GREEN);
  // y += 2;
  
  // print_hex(stack_p8,8,36,y,C_FG_LIGHT_GREEN);

    // poner_string("Stack:", x_base+ancho+3+1, y_base+1, 7, 0);
    // poner_string("(desde tope)", x_base+ancho+3+1, y_base+2, 7, 0);
    uint32_t * stack = (uint32_t * )(esp + 17*4);
    y = 19;
    for(int a = 0; a < 3; a++){// alto*5  = 28*5
        if(esp >= ebp)break;
        print_hex(*stack,8,36,y, C_FG_LIGHT_GREEN);
        ++stack;
        y += 2;
    }

}



void debug_mode_on_off(){
  modoDebug = !modoDebug;

  if (debug_executing && !modoDebug ){
      debug_executing = false;
    
      //restaurar pantalla
      uint32_t* video = (uint32_t*) VIDEO;
      for(uint32_t i = 0; i< 80*41; i++) {
        video[i] = backup_map[i];
    }
  }
    
    if(!modoDebug){
      exception = false;
      print("D",1,48, BLACK_BLACK);
    }

}

void modo_debug(){
  
  if(modoDebug){

    debug_executing = true;
    exception = true;
    
    // hacer backup_pantalla
    uint32_t* video = (uint32_t*) VIDEO;
    for(uint32_t i = 0; i< 80*41; i++) {
      backup_map[i] = video[i];
    }



    pantalla_negra_debug();


    if(ultExcepcion!=260){
      imprimir_excepcion(ultExcepcion);
    }

    // registrosActuales();

    print("stack", 36, 17, C_FG_WHITE);
    // for(int i=0; i<6; i = i + 2){
    //   print_hex(/*proximoStack(i/2)*/ tssActual->esp, 8, 36, i+19, C_FG_LIGHT_GREEN);
    // }

    print("backtrace", 36, 25, C_FG_WHITE);
    print("00000000" , 36, 27, C_FG_LIGHT_GREEN);
    print("00000000" , 36, 29, C_FG_LIGHT_GREEN);
    print("00000000" , 36, 31, C_FG_LIGHT_GREEN);
    print("00000000" , 36, 33, C_FG_LIGHT_GREEN);

    print("cr0", 45, 6, C_FG_WHITE);
    print_hex(rcr0(), 8, 49, 6, C_FG_LIGHT_GREEN);

    print("cr2", 45, 8, C_FG_WHITE);
    print_hex(rcr2(), 8, 49, 8, C_FG_LIGHT_GREEN);

    print("cr3", 45, 10, C_FG_WHITE);
    print_hex(rcr3(),8 , 49, 10, C_FG_LIGHT_GREEN);

    print("cr4", 45, 12, C_FG_WHITE);
    print_hex(rcr4(),8 , 49, 12, C_FG_LIGHT_GREEN);


    print("err", 45, 14, C_FG_WHITE);
    print_hex(codigoError(),8 , 49, 14, C_FG_LIGHT_GREEN);

  }

}
