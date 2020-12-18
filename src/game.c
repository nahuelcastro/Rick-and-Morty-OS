    /* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

#include "colors.h"
#include "mmu.h"
#include "prng.h"
#include "sched.h"
#include "screen.h"
#include "stdint.h"
#include "types.h"

// #include <math.h>
extern void pantalla_negra_debug();

seed semillas[MAX_CANT_SEMILLAS];
uint8_t indexSemilla;
uint16_t cant_semillas;
uint32_t score[PLAYERS];
const bool ADD = true;
const bool DELETE = false;
info_gdt_meeseek info_gdt_meeseeks[GDT_COUNT];


uint8_t color_map_msk[PLAYERS];


bool create_msk_morty = false;
bool create_msk_rick = false;

void clean_cell(coordenadas coord) {
  print("X", coord.x, coord.y, GREEN_GREEN);
}

void update_map_seed(coordenadas coord) {
  // en dicha coord voy a pisar el lugar con el fondo del mapa
  clean_cell(coord);
}

void update_meeseek_map(player_t player, coordenadas coord, bool reason) {
  if (reason) {  // ADD
    uint8_t PLAYER_MEESEEK_COLOR;
    if (player == RICK) {
      PLAYER_MEESEEK_COLOR = RICK_MEESEEK_COLOR;
    } else {
      PLAYER_MEESEEK_COLOR = MORTY_MEESEEK_COLOR;
    }
    print("M", coord.x, coord.y, PLAYER_MEESEEK_COLOR);

  } else {  // reason == DELETE
    clean_cell(coord);
  }
}

void game_init(void) {
  color_map_msk[MORTY] = MORTY_MEESEEK_COLOR;
  color_map_msk[RICK] = RICK_MEESEEK_COLOR;
  ultimoJugador = MORTY;
  tss_creator(RICK, 0);
  tss_creator(MORTY, 0);
  indexSemilla = 0;
  cant_semillas = MAX_CANT_SEMILLAS - 1;
  score[MORTY] = 0;
  score[RICK] = 0;
  print_dec(score[RICK], 8, 10, 45, WHITE_RED);
  print_dec(score[MORTY], 8, 62, 45, WHITE_BLUE);
  print("R  00 01 02 03 04 05 06 07 08 09 ", 23, 43,
        BLACK_RED);                                             // letras rojas
  print("M", 57, 43, BLACK_BLUE);                               // M azul
  print("00 01 02 03 04 05 06 07 08 09 ", 26, 47, BLACK_BLUE);  // letras azules

  // print semillas al azar
  // srand(time(0)); 
  for (int i = 1; i < MAX_CANT_SEMILLAS; i++) {
    uint8_t x = rand();
    uint8_t y = rand();
    print("s", x % 80, (y % 41) + 1, GREEN_YELLOW);
    coordenadas coord;
    coord.x = x % 80;
    coord.y = y % 41 + 1;
    indexSemilla++;
    semillas[indexSemilla].p = true;
    semillas[indexSemilla].coord = coord;
  }

  // setear todos los meeseeks como inactivos
  for (player_t player = 0; player < 2; player++) {
    for (player_t i = 0; i < MAX_CANT_MEESEEKS; i++) {
      meeseeks[player][i].p = 0;
    }
  }

}

void end_game(void) {
  breakpoint();
  pantalla_negra_debug();
  print("FIN DEL JUEGO", 35, 5, C_FG_WHITE);  // ver si quead centrado
  // player_t winner;
  // bool tie = false; 
  if(score[RICK] > score[MORTY]){
    // winner = RICK;
    print("GANADOR: RICK", 35, 20, BLACK_RED);
  } else if(score[RICK] < score[MORTY]){
    // winner = MORTY;
    print("GANADOR: MORTY", 35, 20, BLACK_BLUE);
  } else{
    // tie = true;
    print("EMPATE", 35, 20, BLACK_WHITE);
  }
  while(1){
    breakpoint(); 
  }
  // SI NO HAY EMPATE, PRINT WINNER 
}

void add_update_score(player_t player) {
  score[player] += 425;
  print_dec(score[RICK], 8, 10, 45, WHITE_RED);
  print_dec(score[MORTY], 8, 62, 45, WHITE_BLUE);
}

bool same(coordenadas a, coordenadas b) {
  return (a.x == b.x && a.y == b.y);
}

int8_t next_index_meeseek_free(player_t player) {
  for (int8_t i = 0; i < MAX_CANT_MEESEEKS; i++) {
    if (!meeseeks[player][i].p) {
      return i;
    }
  }
  return -1;  // no entra nunca, pero sino tira warning      // 255 = no existe
              // lugar libre para otro meeseek
}

// busca si la coordenada esta sobre una semilla, ret -1 = no hay semilla en tal
// cordenada, ret x =  en semillas[x] esta la semilla encontrada
int16_t index_in_seed(coordenadas coord) {
  for (int16_t i = 0; i < MAX_CANT_SEMILLAS; i++) {
    if (semillas[i].p) {
      if (same(coord, semillas[i].coord)) {
        return i;
      }
    }
  }
  return -1;
}

void remove_seed(int idx) {
  update_map_seed(semillas[idx].coord);
  semillas[idx].p = false;  // chau semillas                 //! FALTA
                            // ACUTALIZAR EL MAPA(SACAR LA SEMILLA)
}



void msk_found_seed(player_t player, uint8_t idx_msk, int16_t idx_seed) {


  // delete msk
  meeseeks[player][idx_msk].p = false;

  // delete seed
  remove_seed(idx_seed);

  // update
  update_meeseek_map(player, meeseeks[player][idx_msk].coord, DELETE);
  add_update_score(player);
  cant_meeseeks[player]--;
  cant_semillas --;
  info_gdt_meeseeks[tareaActual].ticks_counter = 0;
  meeseeks[player][idx_msk].used_portal_gun = false;
  
  //tareasActivas[tareaActual] = false; 
  info_task[tareaActual].active = false;
  info_task[tareaActual].flag_loop = true;
  sched[player][idx_msk + 1].p_loop_sched = true;

  info_task[tareaActual].clock = 0;
  coordenadas coord; 
  coord.x = 26 + 3 * info_task[tareaActual].idx_msk;
  coord.y = 49 - 4 * info_task[tareaActual].player;
  print("C ", coord.x, coord.y, BLACK_BLACK);
  print("X ", coord.x, coord.y, WHITE_BLACK);
  

  // flag_off  recycling msk memory
  backup_meeseks[player][idx_msk].p = true; /*tenia false, pero tendria que ser true*/


  // //! ESTA M***** PARECE SER LA QUE TIRA EL #UD
  // clean_stack_level_0 para reciclar
  // uint32_t stack = backup_meeseks[player][idx_msk].stack_level_0;
  // stack = stack + PAGE_SIZE - 1;
  // char* ptr_virt_page = (char*)stack;
  // //char* ptr_virt_page = (char*)backup_meeseks[player][idx_msk].stack_level_0;
  // for (int i = 0; i < PAGE_SIZE; i++) {
  //   ptr_virt_page[i] = 0;
  // }
  
  // for (int i = PAGE_SIZE; i < 0; i--) {
  //   ptr_virt_page[i] = 0;
  // }


  // unmap msk
  uint32_t cr3 = rcr3();

  paddr_t virt = backup_meeseks[player][idx_msk].virt;

  // breakpoint();
  for (int i = 0; i < 2; i++) {
    mmu_unmap_page(cr3, virt);
    virt += PAGE_SIZE;
  }
  if(cant_semillas == 0){
      end_game();
  }
  
  
}

// va actualiznado los ticks
void ticks_counter() {
    if (info_gdt_meeseeks[tareaActualAnterior].ticks_counter < 12) { //revisar que funcione a la perfeccion, ver de a una tarea
      info_gdt_meeseeks[tareaActualAnterior].ticks_counter++;
  }
}

int abs(int n) {
  if (n < 0) {
    n = n * (-1);
  }
  return n;
}

void clock_task(){


}





// ;
// in EAX = code Código de la tarea Mr Meeseeks a ser ejecutada.;
// in EBX = x Columna en el mapa donde crear el Mr Meeseeks.;
// in ECX = y Fila en el mapa donde crear el Mr Meeseeks


// code 4KB  Tener en cuenta que este código debe estar declarado dentro del
// espacio de memoria de usuario de la tarea.



uint32_t sys_meeseek(uint32_t code, uint8_t x, uint8_t y) {
  
  player_t player = info_task[tareaActual].player;
    

  if (player != RICK && player != MORTY) {
    return 0;
  }

  // filtro coordenadas validas
  if (x > 80 || y > 40) {
    return 0;
  }

  // filtramos que los meeseeks del jugador no esten en el limite de capacidad
  if (cant_meeseeks[player] >= MAX_CANT_MEESEEKS) {
    return 0;
  }

  coordenadas coord_actual;
  coord_actual.x = x; 
  coord_actual.y = y + 1; //ver si esta bien

  // si meeseek justo cae en semilla, suma puntos y chau semilla
  int index_aux = index_in_seed(coord_actual);
  bool in_seed = index_aux != -1;

  if (in_seed) {
    remove_seed(index_aux);
    add_update_score(player);
    cant_semillas--;
    if(cant_semillas == 0){
      end_game();
    }

    return 0;
  }

  // crear Meeseek
  int8_t index_meeseek = next_index_meeseek_free(player);

  // mapear
  paddr_t virt_res;
  virt_res = tss_meeseeks_creator(player, index_meeseek + 1, code, coord_actual);

  meeseeks[player][index_meeseek].p = 1;
  meeseeks[player][index_meeseek].coord = coord_actual;
  meeseeks[player][index_meeseek].used_portal_gun = false;
  update_meeseek_map(player, coord_actual, ADD);  // 1 = ADD

  cant_meeseeks[player]++;


  return virt_res;
}




uint32_t sys_move(uint32_t x, uint32_t y) {


  // print("task", 19, 31, WHITE_RED);
  // print_dec(tareaActual, 4, 26, 31, WHITE_RED);

  if(tareaActual == 17 || tareaActual == 18 ){ // verificar que funcione
    desactivar_tarea();
  }

  
  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;
  coordenadas coord_actual = meeseeks[player][idx_msk].coord;

  if (x % 80 == 0 && y % 40 == 0) {
    // breakpoint();
    return 0;
  }

  // info_gdt_meeseeks[tareaActual].ticks_counter ++; 

  uint8_t ticks = info_gdt_meeseeks[tareaActual].ticks_counter;
  
   
  uint8_t moveConTicks = (abs(x) + abs(y) + (ticks/2));
  
  if (7 < moveConTicks) {
    return 0;
  }

  clean_cell(coord_actual);

  coordenadas new_coord;
  new_coord.x = x + coord_actual.x;
  new_coord.y = y + coord_actual.y;
  new_coord.x = (new_coord.x % 80 + 80) % 80;
  new_coord.y = ((new_coord.y % 40 + 40) % 41) + 1;
  int16_t index_aux = index_in_seed(new_coord);
  bool in_seed = index_aux != -1;
  
  if (in_seed) {
    msk_found_seed(player, idx_msk, index_aux);
    
    return 1;
  }

  update_meeseek_map(player, new_coord, ADD);
  meeseeks[player][idx_msk].coord = new_coord;

  if (!in_seed) {
    paddr_t new_phy = mmu_phy_map_decoder(new_coord);
    paddr_t virt = backup_meeseks[player][idx_msk].virt;
    mmu_remap_meeseek(new_phy, virt);
  }
  
  
  return 1;
}

//! ESTA TIRANDO PF DESPUES DE VARIOS Y ME PARECE QUE ESTA AGARRANDO SIEMPRE AL MISMO
void move_portal(player_t opponent,uint8_t idx_msk, uint8_t x, uint8_t y){

  player_t player = opponent;
  coordenadas coord_actual = meeseeks[player][idx_msk].coord;

  // player_t other_player = player ? MORTY : RICK;

  clean_cell(coord_actual);

  coordenadas new_coord;
  new_coord.x = x + coord_actual.x;
  new_coord.y = y + coord_actual.y;
  new_coord.x = (new_coord.x % 80 + 80) % 80;
  new_coord.y = ((new_coord.y % 40 + 40) % 41 )+ 1;
  

  int16_t index_aux = index_in_seed(new_coord);
  bool in_seed = index_aux != -1;



// lcr3(cr3[player]);

  if (in_seed) {
    msk_found_seed(player, idx_msk, index_aux);
  }else {
    update_meeseek_map(player, new_coord, ADD);
    meeseeks[player][idx_msk].coord = new_coord;

  
    if (!in_seed) {
      paddr_t new_phy = mmu_phy_map_decoder(new_coord);
      paddr_t virt = backup_meeseks[player][idx_msk].virt;
      mmu_remap_meeseek(new_phy, virt);
    }
  }
  
  
}




// rick y morty la pueden llamar? no aclara
void sys_use_portal_gun(){

  if(tareaActual == 17 || tareaActual == 18 ){ // verificar que funcione
    desactivar_tarea();
  }

  player_t player = info_gdt_meeseeks[tareaActual].player;
  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  bool used_portal_gun = meeseeks[player][idx_msk].used_portal_gun;
  player_t opponent = player ? MORTY : RICK;
  uint8_t number_opp_msks = cant_meeseeks[opponent];

  if(!used_portal_gun && number_opp_msks != 0){

    //busco al azar un meeseek del contrincante que este presente
    uint8_t idxs_msk[number_opp_msks];
    meeseeks[player][idx_msk].used_portal_gun = true;

    // cargo los idxs de los meeseeks activos del oponente
    uint8_t j = 0;
    for (uint8_t i = 0; i < MAX_CANT_MEESEEKS; i++){
      if(meeseeks[opponent][i].p){
        idxs_msk[j] = i;
        j++;
      }
    }

    uint32_t rdm = rand() % number_opp_msks;

    uint8_t idx_msk = idxs_msk[rdm];

    
    //saco una coordenada al azar
    uint32_t x = rand() % 80;
    uint32_t y = (rand() % 41) + 1;

    coordenadas new_coord;
    new_coord.x = (uint8_t) x;  
    new_coord.y = (uint8_t) y;

    coordenadas coord_actual;
    coord_actual = meeseeks[opponent][idx_msk].coord;

    //lo muevo a dicha coordenada, de manera similar al move, es mas, podemos hacer que el move dado un falg de si tiene restriccion o no, mueva
    uint8_t backup_tareaActual = tareaActual;

    tareaActual = meeseeks[opponent][idx_msk].gdt_index;
      
    // print("DISPARA", 10,2,WHITE_RED);
    // print_hex(backup_tareaActual,2, 20, 2,WHITE_RED);

    // print("RECIBE", 25,2,WHITE_RED);
    // print_hex(tareaActual,2, 35,2,WHITE_RED);

      //calculo desplazamiento
    int8_t movement_x = (new_coord.x  - coord_actual.x);  
    int8_t movement_y = (new_coord.y  - coord_actual.y);


    lcr3(cr3[opponent]);

    move_portal(opponent, idx_msk, movement_x,movement_y);

    lcr3(cr3[player]);

    tareaActual = backup_tareaActual; 
   
  }
}


int8_t sys_look (uint8_t flag){

    print("id_m:", 64,30,WHITE_RED);
    print("M:x,y:", 64,32,WHITE_RED);
    print("S:x,y:", 64,34,WHITE_RED);
    print("Mov:", 64,36,WHITE_RED);

  if(tareaActual == 17 || tareaActual == 18 ){ // verificar que funcione
    desactivar_tarea();
  }

  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;

  coordenadas coord_actual = meeseeks[player][idx_msk].coord;

  uint8_t actual_dist;


  uint8_t min_candidate = 255;
  uint32_t min_idx_seed  = MAX_CANT_SEMILLAS + 1; 
  for (uint32_t i = 0; i < MAX_CANT_SEMILLAS; i++){
    if(semillas[i].p){
      actual_dist = abs(coord_actual.x - semillas[i].coord.x) + abs(coord_actual.y - semillas[i].coord.y); 

      // print("S", semillas[i].coord.x, semillas[i].coord.y, WHITE_BLACK);
      // print_dec(semillas[i].coord.x,2, 71,34,WHITE_RED);   //la semilla mas cercana
      // print_dec(semillas[i].coord.y  ,2, 75,34,WHITE_RED);
      // print_dec(actual_dist, 2 ,54, i + 1, RED_GREEN);
      // print_dec(min_candidate, 3 ,76 ,45, WHITE_BLACK);

      // breakpoint();

      // print("s", semillas[i].coord.x, semillas[i].coord.y, GREEN_YELLOW);


      if(actual_dist < min_candidate){
        min_candidate = actual_dist;
        min_idx_seed = i;
      }
    }
  }

  // print_dec(min_candidate, 2 ,76 ,45, WHITE_BLACK);


  
  int16_t movement_x = (int16_t)(semillas[min_idx_seed].coord.x  - coord_actual.x);  
  int16_t movement_y = (int16_t)(semillas[min_idx_seed].coord.y  - coord_actual.y);




  //! NO BORRAR 
  
    
  //   print_dec(idx_msk,2, 71,30,WHITE_RED);                          // para ver si levantamos la tarea que queremos 


  //   print_dec(semillas[min_idx_seed].coord.x,2, 71,34,WHITE_RED);   //la semilla mas cercana
  //   print_dec(semillas[min_idx_seed].coord.y  ,2, 75,34,WHITE_RED);
  //   print("S", semillas[min_idx_seed].coord.x, semillas[min_idx_seed].coord.y, WHITE_BLACK);

  //   if(movement_x < 0){print("-", 70,36,WHITE_RED);}   //la semilla mas cercana
  //   print_dec(abs(movement_x),2, 71,36,WHITE_RED);

  //   if(movement_y < 0){print("-", 74,36,WHITE_RED);}   //la semilla mas cercana
  //   print_dec(abs(movement_y),2, 75,36,WHITE_RED);

  //   breakpoint();

  //   print("-", 70, 36, GREEN_GREEN);
  //   print("-", 74, 36,  GREEN_GREEN);
  //   print("M", coord_actual.x, coord_actual.y, color_map_msk[player]);
  //   print("S", semillas[min_idx_seed].coord.x, semillas[min_idx_seed].coord.y, GREEN_YELLOW);

  // // breakpoint();

  //clean falopa
  for (size_t i = 1; i < 41; i++){
    print_dec(00, 2 ,54, i + 1, GREEN_GREEN);
  }
  
    // breakpoint();

  // breakpoint();
  return flag == 0 ?  movement_x : movement_y;

}




/*

GDT[15]=32-Bit TSS (Available) at 0x00007ee0, length 0x00067   INICIAL
GDT[16]=32-Bit TSS (Busy) at 0x00007e60, length 0x00067        IDLE
GDT[17]=32-Bit TSS (Available) at 0x000079e0, length 0x00067   RICK
GDT[18]=32-Bit TSS (Available) at 0x00007560, length 0x00067   MORTY

GDT[19]=32-Bit TSS (Available) at 0x00007a48, length 0x00067
GDT[20]=32-Bit TSS (Available) at 0x000075c8, length 0x00067
GDT[21]=32-Bit TSS (Available) at 0x00007ab0, length 0x00067
GDT[22]=32-Bit TSS (Available) at 0x00007630, length 0x00067
GDT[23]=32-Bit TSS (Available) at 0x00007698, length 0x00067
GDT[24]=32-Bit TSS (Available) at 0x00007700, length 0x00067
GDT[25]=32-Bit TSS (Available) at 0x00007768, length 0x00067
GDT[26]=32-Bit TSS (Available) at 0x000077d0, length 0x00067
GDT[27]=32-Bit TSS (Available) at 0x00007838, length 0x00067
GDT[28]=32-Bit TSS (Available) at 0x000078a0, length 0x00067
GDT[29]=32-Bit TSS (Available) at 0x00007908, length 0x00067
GDT[30]=32-Bit TSS (Available) at 0x00007970, length 0x00067



! COSAS QUE HACER (MAS RANCIO ESTO, MARCO NO VEAS ESTO):
TODO. VER LO DE BORRAR EL STACK DE LEVEL 0
TODO. TERMINAR LA IMPLEMENTACION PARA QUE PUEDA REUTILIZAR MEESEEKS
TODO. TERMINAR DEBUGGER
TODO. VER EL LOOK QUE PARECE ANDAR MEDIO FALOPA ;)
X TODO. LO DE LOS RELOJITOS // HECHO 
x TODO. TERMINAR EL FIN DE JUEGO (y que diga quien gano) // HECHO

TODO. VER QUE CUANDO LE DIGO QUE SE CREEN 20 SEMILLAS (MAX CANT = 21) TIRA PAGE FAULT EN EL ULTIMO CASO :(





*/
