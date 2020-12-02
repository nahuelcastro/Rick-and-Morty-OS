/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "prng.h"
#include "types.h"
#include "colors.h"
#include "screen.h"

#include "stdint.h"

#define MAX_CANT_SEMILLAS 50
#define WHITE_RED 0x4f
#define WHITE_BLUE 0x1f
#define BLACK_RED 0x04
#define BLACK_BLUE 0x01
#define GREEN_YELLOW 0x2E

extern void pantalla_negra_debug();


typedef struct{
    int x;
    int y;
} coordenadas;


coordenadas semillas[MAX_CANT_SEMILLAS];
int indexSemilla;


void game_init(void) {
    print("00000000",10, 43, WHITE_RED);  // puntajes rojos en 0
    print("00000000",62, 43, WHITE_BLUE); // puntajes azules en 0
    print("R  00  01  02  03  04  05  06  07  ",22, 42,BLACK_RED); // letras rojas
    print("M",57, 42, BLACK_BLUE); // M azul
    print("00  01  02  03  04  05  06  07  ",25, 45,BLACK_BLUE); // letras azules

    indexSemilla = 0;
    for(int i=1; i < MAX_CANT_SEMILLAS; i++){
        int x = rand();
        int y = rand();
        print("s", x % 79, y % 40, GREEN_YELLOW);
        indexSemilla++;
        semillas[indexSemilla].x = x;
        semillas[indexSemilla].y = y;
    }
    
}

void end_game(){
    pantalla_negra_debug();
    print("FIN DEL JUEGO", 35, 5, C_FG_WHITE); // ver si quead centrado
}