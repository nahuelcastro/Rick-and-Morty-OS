/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas asociadas al juego.
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "types.h"

//#include <stdbool.h>

typedef enum e_task_type {
  Rick = 1,
  Morty = 2,
  Meeseeks = 3,
} task_type_t;

typedef struct coordenadas{
  uint8_t x;
  uint8_t y;
} coordenadas;

typedef struct seed{
  coordenadas coord;
  _Bool p;
} seed;

extern seed semillas[MAX_CANT_SEMILLAS];
extern uint8_t indexSemilla;
extern uint16_t cant_semillas;
extern uint32_t score[PLAYERS]; // score[player]

void game_init(void);
void end_game(void);

void update_map(void);
void update_seed(void);

void add_update_score(player_t player);
_Bool same(coordenadas a, coordenadas b);
int8_t next_index_meeseek_free(player_t player);
int index_in_seed(coordenadas coord);
uint32_t create_meeseek(uint32_t code, uint8_t x, uint8_t y);
void bye_seed(int idx);

#endif //  __GAME_H__
