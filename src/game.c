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

extern void pantalla_negra_debug();

seed semillas[MAX_CANT_SEMILLAS];

uint16_t cant_semillas;
uint32_t score[PLAYERS];
const bool ADD = true;
const bool DELETE = false;
info_gdt_meeseek info_gdt_meeseeks[GDT_COUNT];
look_t  looks[PLAYERS][MAX_CANT_MEESEEKS];
bool moves[PLAYERS][MAX_CANT_MEESEEKS];
uint32_t ret_meeseek[PLAYERS][MAX_CANT_MEESEEKS];


bool create_msk_morty = false;
bool create_msk_rick = false;

void clean_cell(coordenadas coord) {
  print("X", coord.x, coord.y + 1, GREEN_GREEN);
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
    print("M", coord.x, coord.y + 1, PLAYER_MEESEEK_COLOR);

  } else {  // reason == DELETE
    clean_cell(coord);
  }
}

void game_init(void) {

  // breakpoint();

  modoDebug = true;

  tss_creator(RICK);
  tss_creator(MORTY);
  cant_semillas = MAX_CANT_SEMILLAS;
  score[MORTY] = 0;
  score[RICK] = 0;

  print_dec(score[RICK], 8, 10, 44, WHITE_RED);
  print_dec(score[MORTY], 8, 62, 44, WHITE_BLUE);

  print("R  00 01 02 03 04 05 06 07 08 09 ", 23, 42, BLACK_RED); // letras rojas
  print("M", 57, 42, BLACK_BLUE);                                // M azul
  print("00 01 02 03 04 05 06 07 08 09 ", 26, 46, BLACK_BLUE);   // letras azules

  // print semillas al azar
  for (int i = 0; i < MAX_CANT_SEMILLAS; i++) {
    uint8_t x = rand();
    uint8_t y = rand();
    print("s", x % 80, (y % 40) + 1, GREEN_YELLOW);
    coordenadas coord;
    coord.x = x % 80;
    coord.y = y % 40;
    semillas[i].p = true;
    semillas[i].coord = coord;
  }

  // setear todos los meeseeks como inactivos
  for (player_t player = 0; player < 2; player++) {
    for (player_t i = 0; i < MAX_CANT_MEESEEKS; i++) {
      meeseeks[player][i].p = 0;
    }
  }

  for (size_t i = 0; i < 2; i++){
    for (size_t j = 0; j < MAX_CANT_MEESEEKS; j++){
      looks[i][j].x = 0;
      looks[i][j].y = 0;
      moves[i][j] = 0;
      ret_meeseek[i][j] = 0;
    }
    
  }
}

void end_game(void) {
  pantalla_negra_debug();
  print("END GAME", 36, 5, C_FG_WHITE);  

  if(score[RICK] > score[MORTY]){
    print("WINNER: RICK ", 34, 20, WHITE_BLACK);
    print("XXXXXXXXXXXXXXXXX", 32, 18, RED_RED);
    print("XXXXXXXXXXXXXXXXX", 32, 22, RED_RED);
    for (int i = 19; i < 22; i++){
      print("X", 32, i, RED_RED);
      print("X", 48, i, RED_RED);
    }
  } else if(score[RICK] < score[MORTY]){
    
    print("WINNER: MORTY", 34, 20, WHITE_BLACK);
    print("XXXXXXXXXXXXXXXXX", 32, 18, BLUE_BLUE);
    print("XXXXXXXXXXXXXXXXX", 32, 22, BLUE_BLUE);
    for (int i = 19; i < 22; i++){
      print("X", 32, i, BLUE_BLUE);
      print("X", 48, i, BLUE_BLUE);
    }
  

  } else{
    print("TIE", 38, 20, BLACK_WHITE);
  }
  while(1){
    breakpoint(); 
  }
}

void add_update_score(player_t player) {
  score[player] += 425;
  print_dec(score[RICK], 8, 10, 44, WHITE_RED);
  print_dec(score[MORTY], 8, 62, 44, WHITE_BLUE);

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
  return -1;  // no entra nunca, pero sino tira warning 
}

// busca si la coordenada esta sobre una semilla, ret -1 = no hay semilla en tal
// cordenada, ret x =  en semillas[x] esta la semilla encontrada
int16_t coord_in_seed(coordenadas coord) {
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
  semillas[idx].p = false;  
  cant_semillas--;          
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
  info_gdt_meeseeks[tareaActual].ticks_counter = 0;
  meeseeks[player][idx_msk].used_portal_gun = false;
  info_task[tareaActual].active = false;
  // info_task[tareaActual].flag_loop = true;
  info_task[tareaActual].clock = 0;


  // se marca la X en el tablero    
  coordenadas coord; 
  coord.x = 26 + 3 * info_task[tareaActual].idx_msk;
  coord.y = 48 - 4 * info_task[tareaActual].player;
  print("C ", coord.x, coord.y, BLACK_BLACK);
  print("X ", coord.x, coord.y, WHITE_BLACK);

  // flag_off  recycling msk memory
  backup_meeseeks[player][idx_msk].p = true;

  // unmap msk
  uint32_t cr3 = rcr3();

  paddr_t virt = backup_meeseeks[player][idx_msk].virt;

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
    if (info_gdt_meeseeks[tareaActualAnterior].ticks_counter < 12) { 
      info_gdt_meeseeks[tareaActualAnterior].ticks_counter++;
  }
}

int abs(int n) {
  if (n < 0) {
    n = n * (-1);
  }
  return n;
}


// uint32_t sys_meeseek(uint32_t code, uint8_t x, uint8_t y) {

//   player_t player = info_task[tareaActual].player;
    
//   if (player != RICK && player != MORTY) {
//     return 0;
//   }

//   // filtro coordenadas validas
//   if (x > 80 || y > 40) {
    
//     return 0;
//   }

//   // filtramos que los meeseeks del jugador no esten en el limite de capacidad
//   if (cant_meeseeks[player] >= MAX_CANT_MEESEEKS) {
//     return 0;
//   }

//   coordenadas coord_actual;
//   coord_actual.x = x; 
//   coord_actual.y = y; 

//   // si meeseek justo cae en semilla, suma puntos y chau semilla
//   int index_aux = coord_in_seed(coord_actual);
//   bool in_seed = index_aux != -1;

//   if (in_seed) {
//     remove_seed(index_aux);
//     add_update_score(player);
//     if(cant_semillas == 0){
//       end_game();
//     }
//     return 0;
//   }

//   // crear Meeseek
//   int8_t index_meeseek = next_index_meeseek_free(player);

//   // mapear
//   paddr_t virt_res;
//   virt_res = tss_meeseeks_creator(player, index_meeseek, code, coord_actual);

//   update_meeseek_map(player, coord_actual, ADD);  // 1 = ADD

//   return virt_res;
// }


void sys_meeseek(uint32_t code, uint8_t x, uint8_t y) {

  player_t player = info_task[tareaActual].player;
  int8_t index_meeseek = next_index_meeseek_free(player);
    
  if (player != RICK && player != MORTY) {
    return;
  }

  // filtro coordenadas validas
  if (x > 80 || y > 40) {
    ret_meeseek[player][index_meeseek] = 0;
    return;
  }

  // filtramos que los meeseeks del jugador no esten en el limite de capacidad
  if (cant_meeseeks[player] >= MAX_CANT_MEESEEKS) {
    ret_meeseek[player][index_meeseek] = 0;
    return;
  }

  coordenadas coord_actual;
  coord_actual.x = x; 
  coord_actual.y = y; 

  // si meeseek justo cae en semilla, suma puntos y chau semilla
  int index_aux = coord_in_seed(coord_actual);
  bool in_seed = index_aux != -1;

  if (in_seed) {
    remove_seed(index_aux);
    add_update_score(player);
    if(cant_semillas == 0){
      end_game();
    }
    ret_meeseek[player][index_meeseek] = 0;
    return;
  }

  // mapear
  paddr_t virt_res;
  virt_res = tss_meeseeks_creator(player, index_meeseek, code, coord_actual);

  update_meeseek_map(player, coord_actual, ADD);  // 1 = ADD

  ret_meeseek[player][index_meeseek] = virt_res;
  return;
}






void print_temps(int8_t x, int8_t y){
  player_t player = info_gdt_meeseeks[tareaActual].player;
  if (player){
    print_dec(abs(x), 2, 73, 42, RICK_MEESEEK_COLOR);
    print_dec(abs(y), 2, 77, 42, RICK_MEESEEK_COLOR);
    if (x<0){
      print("-", 72, 42, RICK_MEESEEK_COLOR);
    } else{
      print("-", 72, 42, BLACK_BLACK);
    }
    if (y<0){
      print("-", 76, 42, RICK_MEESEEK_COLOR);
    } else{
      print("-", 76, 42, BLACK_BLACK);
    }
    
    
  }else{
    print_dec(abs(x), 2, 73, 44, MORTY_MEESEEK_COLOR);
    print_dec(abs(y), 2, 77, 44, MORTY_MEESEEK_COLOR);
    if (x<0){
      print("-", 72, 44, MORTY_MEESEEK_COLOR);
    } else{
      print("-", 72, 44, BLACK_BLACK);
    }
    if (y<0){
      print("-", 76, 44, MORTY_MEESEEK_COLOR);
    } else{
      print("-", 76, 44, BLACK_BLACK);
    }
  }
  // breakpoint();

}



void sys_move(int32_t x, int32_t y) {

  if(tareaActual < 0x13 || tareaActual > 0x26 ){
    desactivar_tarea();
  }

  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;
  coordenadas coord_actual = meeseeks[player][idx_msk].coord;
  

  if (x % 80 == 0 && y % 40 == 0) {
    moves[player][idx_msk] = false;
    return;
  }

  uint8_t ticks = info_gdt_meeseeks[tareaActual].ticks_counter;
   
  uint8_t moveConTicks = (abs(x) + abs(y) + (ticks/2));
  
  if (7 < moveConTicks) {
    moves[player][idx_msk] = false;
    return;
  }

  clean_cell(coord_actual);

  coordenadas new_coord;

  new_coord = new_position(coord_actual,x,y);

  int16_t index_aux = coord_in_seed(new_coord);
  bool in_seed = index_aux != -1;
  
  if (in_seed) {
    msk_found_seed(player, idx_msk, index_aux);
    moves[player][idx_msk] = true;
    return;
  }

  update_meeseek_map(player, new_coord, ADD);
  meeseeks[player][idx_msk].coord = new_coord;

  if (!in_seed) {

    paddr_t new_phy = mmu_phy_map_decoder(new_coord);
    paddr_t virt = backup_meeseeks[player][idx_msk].virt;

    mmu_remap_meeseek(new_phy, virt);
  }

  moves[player][idx_msk] = true;
  return;
}




// void sys_move(int32_t x, int32_t y) {

//   if(tareaActual < 0x13 || tareaActual > 0x26 ){
//     desactivar_tarea();
//   }

//   uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
//   player_t player = info_gdt_meeseeks[tareaActual].player;
//   coordenadas coord_actual = meeseeks[player][idx_msk].coord;
  

//   if (x % 80 == 0 && y % 40 == 0) {
//     moves[player][idx_msk] = false;
//     return 0;
//   }

//   uint8_t ticks = info_gdt_meeseeks[tareaActual].ticks_counter;
   
//   uint8_t moveConTicks = (abs(x) + abs(y) + (ticks/2));
  
//   if (7 < moveConTicks) {
//     moves[player][idx_msk] = false;
//     return 0;
//   }

//   clean_cell(coord_actual);

//   coordenadas new_coord;

//   new_coord = new_position(coord_actual,x,y);

//   int16_t index_aux = coord_in_seed(new_coord);
//   bool in_seed = index_aux != -1;
  
//   if (in_seed) {
//     msk_found_seed(player, idx_msk, index_aux);
//     moves[player][idx_msk] = true;
//     return 1;
//   }

//   update_meeseek_map(player, new_coord, ADD);
//   meeseeks[player][idx_msk].coord = new_coord;

//   if (!in_seed) {

//     paddr_t new_phy = mmu_phy_map_decoder(new_coord);
//     paddr_t virt = backup_meeseeks[player][idx_msk].virt;

//     mmu_remap_meeseek(new_phy, virt);
//   }

//   moves[player][idx_msk] = true;
//   return 1;
// }







void move_portal(player_t opponent,uint8_t idx_msk, int8_t x, int8_t y){

  player_t player = opponent;
  coordenadas coord_actual = meeseeks[player][idx_msk].coord;

  clean_cell(coord_actual);

  coordenadas new_coord;

  new_coord = new_position(coord_actual,x,y);

  int16_t index_aux = coord_in_seed(new_coord);
  bool in_seed = index_aux != -1;

  if (in_seed) {
    msk_found_seed(player, idx_msk, index_aux);
  }else {
    update_meeseek_map(player, new_coord, ADD);
    meeseeks[player][idx_msk].coord = new_coord;
  
    if (!in_seed) {
      paddr_t new_phy = mmu_phy_map_decoder(new_coord);
      paddr_t virt = backup_meeseeks[player][idx_msk].virt;
      mmu_remap_meeseek(new_phy, virt);
    }
  } 
}



void sys_use_portal_gun(){

  if(tareaActual < 0x13 || tareaActual > 0x26 ){
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
    uint32_t y = (rand() % 40);

    coordenadas new_coord;
    new_coord.x = (uint8_t) x;  
    new_coord.y = (uint8_t) y;

    coordenadas coord_actual;
    coord_actual = meeseeks[opponent][idx_msk].coord;

    //lo muevo a dicha coordenada, de manera similar al move, es mas, podemos hacer que el move dado un falg de si tiene restriccion o no, mueva
    uint8_t backup_tareaActual = tareaActual;

    tareaActual = meeseeks[opponent][idx_msk].gdt_index;
      
    //calculo desplazamiento
    int8_t movement_x = (new_coord.x  - coord_actual.x);  
    int8_t movement_y = (new_coord.y  - coord_actual.y);


    lcr3(cr3s[opponent]);

    move_portal(opponent, idx_msk, movement_x,movement_y);

    lcr3(cr3s[player]);

    tareaActual = backup_tareaActual; 


  }
}


void sys_look (){

  if (!(tareaActual == 17) && !(tareaActual == 18)){
    
    uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
    player_t player = info_gdt_meeseeks[tareaActual].player;
    coordenadas coord_actual = meeseeks[player][idx_msk].coord;

    uint8_t actual_dist;
    uint8_t min_candidate = 255;
    uint32_t min_idx_seed  = MAX_CANT_SEMILLAS;
    for (uint32_t i = 0; i < MAX_CANT_SEMILLAS; i++){
      if(semillas[i].p){
        actual_dist = abs(coord_actual.x - semillas[i].coord.x) + abs(coord_actual.y - semillas[i].coord.y); 
        if(actual_dist < min_candidate){
          min_candidate = actual_dist;
          min_idx_seed = i;
        }
      }
    }
    
    int8_t movement_x = (int8_t)(semillas[min_idx_seed].coord.x  - coord_actual.x);  
    int8_t movement_y = (int8_t)(semillas[min_idx_seed].coord.y  - coord_actual.y);

    // cargar a estructura
    looks[player][idx_msk].x = movement_x;
    looks[player][idx_msk].y = movement_y;  
  }

}


coordenadas new_position(coordenadas actual,int8_t x,int8_t y){

  uint8_t dist_x = abs(x);
  uint8_t dist_y = abs(y);

  for (size_t i = 0; i < dist_x; i++){
    if(x > 0){
      if(actual.x == 79){
        actual.x = 0;
      }else{
        actual.x++;
        x--;
      }

    }else{
      if(actual.x == 0){
        actual.x = 79;
      }else{
        actual.x--;
        x--;
      }
    }

  }

  for (size_t i = 0; i < dist_y; i++){
      if(y > 0){
        if(actual.y == 39){
          actual.y = 0;
        }else{
          actual.y++;
          y--;
        }

      }else{
        if(actual.y == 0){
          actual.y = 39;
        }else{
          actual.y--;
          y--;
        }
      }
    }
    return actual;
}
  

int8_t take_look(bool flag){

  if(tareaActual == 17 || tareaActual == 18){
    int8_t movement_x = -1;
    int8_t movement_y = -1;
    return flag == 0 ?  movement_x : movement_y;
  }
  
  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;

  int8_t movement_x = looks[player][idx_msk].x;
  int8_t movement_y = looks[player][idx_msk].y;
  
  return flag == 0 ?  movement_x : movement_y;
}

bool take_move(){
  
  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;

  return moves[player][idx_msk];
  
  
}


bool take_meeseek(){
  
  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;

  return ret_meeseek[player][idx_msk];
  
  
}