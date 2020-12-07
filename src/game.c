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
uint8_t indexSemilla;
uint16_t cant_semillas;
uint32_t score[PLAYERS];
const bool ADD = true;
const bool DELETE = false;
info_gdt_meeseek info_gdt_meeseeks[GDT_COUNT];

// void update_map(void){
//     update_seed();
// }

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
    // breakpoint();
    print("M", coord.x, coord.y + 1, PLAYER_MEESEEK_COLOR);
  } else {  // reason == DELETE
    // breakpoint();
    clean_cell(coord);
  }
}

void game_init(void) {
  ultimoJugador = MORTY;
  tss_creator(RICK, 0);
  tss_creator(MORTY, 0);
  indexSemilla = 0;
  score[MORTY] = 0;
  score[RICK] = 0;
  print_dec(score[RICK], 8, 10, 44, WHITE_RED);
  print_dec(score[MORTY], 8, 62, 44, WHITE_BLUE);
  // print("00000000",10, 43, WHITE_RED);  // puntajes rojos en 0
  // print("00000000",62, 43, WHITE_BLUE); // puntajes azules en 0
  print("R  00 01 02 03 04 05 06 07 08 09 ", 23, 42,
        BLACK_RED);                                             // letras rojas
  print("M", 57, 42, BLACK_BLUE);                               // M azul
  print("00 01 02 03 04 05 06 07 08 09 ", 26, 46, BLACK_BLUE);  // letras azules

  // print semillas al azar
  for (int i = 1; i < MAX_CANT_SEMILLAS; i++) {
    uint8_t x = rand();
    uint8_t y = rand();
    print("s", x % 80, (y % 40) + 1, GREEN_YELLOW);
    coordenadas coord;
    coord.x = x % 80;
    coord.y = y % 40;
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
  // breakpoint();
  // pantalla_negra_debug();
  print("FIN DEL JUEGO", 35, 5, C_FG_WHITE);  // ver si quead centrado
}

void add_update_score(player_t player) {
  score[player] += 425;
  print_dec(score[RICK], 8, 10, 44, WHITE_RED);
  print_dec(score[MORTY], 8, 62, 44, WHITE_BLUE);
}

bool same(coordenadas a, coordenadas b) {
  // print_dec(a.x, 2 ,2,  i - 1, WHITE_RED);
  // print_dec(a.y, 2 ,6,  i - 1, WHITE_RED);
  // print_dec(b.x, 2 ,12, i - 1, WHITE_RED);
  // print_dec(b.y, 2 ,18, i - 1, WHITE_RED);
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
int index_in_seed(coordenadas coord) {
  for (int i = 0; i < MAX_CANT_SEMILLAS; i++) {
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

void msk_found_seed(player_t player, int idx_msk, int idx_seed) {
  breakpoint();

  // delete msk
  meeseeks[player][idx_msk].p = false;

  // delete seed
  remove_seed(idx_seed);

  // update
  update_meeseek_map(player, meeseeks[player][idx_msk].coord, DELETE);
  add_update_score(player);
  cant_meeseeks[player]--;
  info_gdt_meeseeks[tareaActual].ticks_counter = 0;
  tareasActivas[tareaActual] = false;

  // flag_off  recycling msk memory
  backup_meeseks[player][idx_msk].p = false;

  // clean_stack_level_0 para reciclar
  char* ptr_virt_page = (char*)backup_meeseks[player][idx_msk].stack_level_0;
  for (int i = 0; i < PAGE_SIZE; i++) {
    ptr_virt_page[i] = 0;
  }


  // unmap msk
  uint32_t cr3 = rcr3();

  paddr_t virt = backup_meeseks[player][idx_msk].virt;

  for (int i = 0; i < 2; i++) {
    mmu_unmap_page(cr3, virt);
    virt += PAGE_SIZE;
  }

  breakpoint();
}

// ;
// in EAX = code Código de la tarea Mr Meeseeks a ser ejecutada.;
// in EBX = x Columna en el mapa donde crear el Mr Meeseeks.;
// in ECX = y Fila en el mapa donde crear el Mr Meeseeks

// code 4KB  Tener en cuenta que este código debe estar declarado dentro del
// espacio de memoria de usuario de la tarea.

//! MAXI NO ME SAQUES ESTOS COMENTARIOS POR FAVOR :)
//! ACORDARSE DE VER LO DE LA TAREA IDLE, CUANDO TERMINA DE EJECUTAR ALGUNA
//! TAREA
//! Por otro lado, ninguno de los servicios debe modicar ningún registro, a
//! excepción de los indicados anteriormente.Tener en consideración que luego
//! del llamado a cualquiera de los servicios, la tarea en ejecución pierde el
//! turno.Es decir, que es desalojada del scheduler, y debe esperar hasta que
//! pueda ser ejecutada nuevamente.En el tiempo entre que la tarea es desalojada
//! y llega una interrupción de reloj para ejecutar la próxima tarea, el sistema
//! debe ejecutar a la tarea Idle.Esta última, no tiene más propósito que
//! mantener al procesador realizando alguna tarea, a la espera del próximo
//! punto de sincronismo.

//! ACORDARSE DE VER LO DE LA TAREA IDLE, CUANDO TERMINA DE EJECUTAR ALGUNA
//! TAREA
//! Por otro lado, ninguno de los servicios debe modicar ningún registro, a
//! excepción de los indicados anteriormente.Tener en consideración que luego
//! del llamado a cualquiera de los servicios, la tarea en ejecución pierde el
//! turno.Es decir, que es desalojada del scheduler, y debe esperar hasta que
//! pueda ser ejecutada nuevamente.En el tiempo entre que la tarea es desalojada
//! y llega una interrupción de reloj para ejecutar la próxima tarea, el sistema
//! debe ejecutar a la tarea Idle.Esta última, no tiene más propósito que
//! mantener al procesador realizando alguna tarea, a la espera del próximo
//! punto de sincronismo.

//! ACORDARSE DE VER LO DE LA TAREA IDLE, CUANDO TERMINA DE EJECUTAR ALGUNA
//! TAREA
//! Por otro lado, ninguno de los servicios debe modicar ningún registro, a
//! excepción de los indicados anteriormente.Tener en consideración que luego
//! del llamado a cualquiera de los servicios, la tarea en ejecución pierde el
//! turno.Es decir, que es desalojada del scheduler, y debe esperar hasta que
//! pueda ser ejecutada nuevamente.En el tiempo entre que la tarea es desalojada
//! y llega una interrupción de reloj para ejecutar la próxima tarea, el sistema
//! debe ejecutar a la tarea Idle.Esta última, no tiene más propósito que
//! mantener al procesador realizando alguna tarea, a la espera del próximo
//! punto de sincronismo.

uint32_t create_meeseek(uint32_t code, uint8_t x, uint8_t y) {
  // breakpoint();
  player_t player = player_idx_gdt[tareaActual];

  // filtro jugador                               VEER SI NO ES MEJOR FILTRAR
  // ANTES (juan dice creo que con filtar aca esta bien)
  if (player != RICK && player != MORTY) {
    return 0;
  }

  // filtro coordenadas validas
  if (x > 80 || y > 40) {
    return 0;
  }

  // filtramos que los meeseeks del jugador no esten en el limite de capacidad
  if (cant_meeseeks[player] == MAX_CANT_MEESEEKS) {
    return 0;
  }

  coordenadas coord_actual;
  coord_actual.x = x;
  coord_actual.y = y;

  // si meeseek justo cae en semilla, suma puntos y chau semilla
  int index_aux = index_in_seed(coord_actual);
  bool in_seed = index_aux != -1;

  // print_dec(in_seed, 8, 10, 7, WHITE_RED);
  if (in_seed) {
    remove_seed(index_aux);
    add_update_score(player);
    return 0;
  }

  // crear Meeseek
  int8_t index_meeseek = next_index_meeseek_free(player);

  // mapear
  paddr_t virt_res;
  virt_res =
      tss_meeseeks_creator(player, index_meeseek + 1, code, coord_actual);

  meeseeks[player][index_meeseek].p = 1;
  meeseeks[player][index_meeseek].coord = coord_actual;
  update_meeseek_map(player, coord_actual, ADD);  // 1 = ADD

  cant_meeseeks[player]++;

  return virt_res;
}





uint32_t sys_move(uint32_t x, uint32_t y) {

  if(tareaActual == 17 | tareaActual == 18 ){ // verificar que funcione
    desactivar_tarea();
  }


  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;

  coordenadas coord_actual = meeseeks[player][idx_msk].coord;

  if (x % 80 == 0 && y % 40 == 0) {
    return 0;
  }

  // info_gdt_meeseeks[tareaActual].ticks_counter ++; 

  uint8_t ticks = info_gdt_meeseeks[tareaActual].ticks_counter;
  // print_hex(tareaActual, 4, 8, 31, WHITE_RED);
  // print_hex(info_gdt_meeseeks[tareaActual].ticks_counter, 4, 8, 30, WHITE_RED);

   

  uint8_t moveConTicks = (abs(x) + abs(y) + (ticks/2));
  
  if (7 < moveConTicks) {
    return 0;
  }
  coordenadas new_coord;
  new_coord.x = x + coord_actual.x;
  new_coord.y = y + coord_actual.y;
  new_coord.x = (new_coord.x % 80 + 80) % 80;
  new_coord.y = (new_coord.y % 40 + 40) % 40;
  clean_cell(coord_actual);

  int index_aux = index_in_seed(coord_actual);
  bool in_seed = index_aux != -1;
  

  if (in_seed) {
    msk_found_seed(player, idx_msk, index_aux);
    breakpoint();

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


int16_t sys_look (){

  uint8_t idx_msk = info_gdt_meeseeks[tareaActual].idx_msk;
  player_t player = info_gdt_meeseeks[tareaActual].player;

  coordenadas coord_actual = meeseeks[player][idx_msk].coord;

  uint8_t min_candidate = 255;
  uint32_t min_idx_seed  = MAX_CANT_SEMILLAS + 1; 
  for (uint32_t i = 0; i < MAX_CANT_SEMILLAS; i++){
    if(semillas[i].p){
      uint8_t actual_dist = abs(coord_actual.x - semillas[i].coord.x) + abs(coord_actual.y - semillas[i].coord.y); 
      if(actual_dist < min_candidate){
        min_candidate = actual_dist;
        min_idx_seed = i;
      }
    }
  }
  
  //ya tenemos la i de la semilla
  int16_t displacement_x = (int16_t)(semillas[min_idx_seed].coord.x  - coord_actual.x);  
  int16_t displacement_y = (int16_t)(semillas[min_idx_seed].coord.y  - coord_actual.y);

  int16_t res = 0;
  res = displacement_x;     //FFFF FFFC   , FFFC
  res << 8;
  displacement_y = displacement_y && 0x00FF;
  res = res || displacement_y;

  return res;

}



  // print_hex(tareaActual, 4, 8, 30, WHITE_RED);
  // print_hex(moveConTicks, 4, 8, 40, WHITE_RED);