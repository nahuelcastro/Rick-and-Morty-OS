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


typedef enum e_task_type {
  Rick = 1,
  Morty = 2,
  Meeseeks = 3,
} task_type_t;

typedef struct coordenadas{
  uint8_t x;
  uint8_t y;
} coordenadas;

typedef struct ret_2{
  int8_t x;
  int8_t y;
} ret_2;

typedef struct seed{
  coordenadas coord;
  bool p;
} seed;

typedef struct meeseeks_info{

  uint8_t idx_msk; 
  player_t player;
  uint32_t ticks_counter;

} info_gdt_meeseek;

extern seed semillas[MAX_CANT_SEMILLAS];
extern uint8_t indexSemilla;
extern uint16_t cant_semillas;
extern uint32_t score[PLAYERS];

extern bool create_msk_morty;
extern bool create_msk_rick;


void game_init(void);
void end_game(void);

void update_map(void);
void update_seed(void);

void add_update_score(player_t player);
_Bool same(coordenadas a, coordenadas b);
int8_t next_index_meeseek_free(player_t player);
int16_t coord_in_seed(coordenadas coord);
void remove_seed(int idx);
void ticks_counter(void);
int abs(int n);
bool check_end_game();
void clock_task();

void msk_found_seed(player_t player, uint8_t idx_msk, int16_t idx_seed);
void move_portal(player_t opponent,uint8_t idx_msk, int8_t x, int8_t y);

void sys_meeseek(uint32_t code, uint8_t x, uint8_t y);
void  sys_move(int32_t x, int32_t y);
void sys_use_portal_gun();

void print_aux(int8_t deltax, int8_t deltay);

extern info_gdt_meeseek info_gdt_meeseeks[GDT_COUNT];

extern  coordenadas new_position(coordenadas actual,int8_t x,int8_t y);


void print_temps(int8_t x, int8_t y);

typedef struct look{
  int8_t x;
  int8_t y;
}look_t;

extern look_t looks[PLAYERS][MAX_CANT_MEESEEKS];
extern bool moves[PLAYERS][MAX_CANT_MEESEEKS];
extern uint32_t ret_meeseek[PLAYERS][MAX_CANT_MEESEEKS];

#endif //  __GAME_H__


