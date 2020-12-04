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
#include "sched.h"
#include "mmu.h"

#include "stdint.h"
extern void pantalla_negra_debug();

seed semillas[MAX_CANT_SEMILLAS];
uint8_t indexSemilla;
uint16_t cant_semillas;
uint32_t score[2];


// void update_map(void){
//     update_seed();
// }

void clean_cell(coordenadas coord){
    print("X", coord.x, coord.y, GREEN_GREEN);
}

void update_map_seed(coordenadas coord){
    // en dicha coord voy a pisar el lugar con el fondo del mapa
    clean_cell(coord);
}

void update_meeseek_map(int8_t player, coordenadas coord,uint8_t reason){
    
    if (reason){
        uint8_t PLAYER_MEESEEK_COLOR;
        if (player == RICK){   
            PLAYER_MEESEEK_COLOR = RICK_MEESEEK_COLOR; 
        } else{
            PLAYER_MEESEEK_COLOR = MORTY_MEESEEK_COLOR;
        }
        print("M", coord.x, coord.y, PLAYER_MEESEEK_COLOR);
    } else{ // reason == DELETE
        clean_cell(coord);
    }
    
}

void game_init(void) {
    indexSemilla = 0;
    score[MORTY] = 0;
    score[RICK]  = 0;
    print_dec(score[RICK], 8, 10, 43, WHITE_RED);
    print_dec(score[MORTY], 8, 62, 43, WHITE_BLUE);
    // print("00000000",10, 43, WHITE_RED);  // puntajes rojos en 0
    // print("00000000",62, 43, WHITE_BLUE); // puntajes azules en 0
    print("R  00 01 02 03 04 05 06 07 08 09 ",23, 41,BLACK_RED); // letras rojas
    print("M",57, 41, BLACK_BLUE); // M azul
    print("00 01 02 03 04 05 06 07 08 09 ",26, 45,BLACK_BLUE); // letras azules



    

    // print semillas al azar
    for(int i=1; i < MAX_CANT_SEMILLAS; i++){
        uint8_t x = rand();
        uint8_t y = rand();
        print("s", x % 79, y % 40, GREEN_YELLOW);
        coordenadas coord;
        coord.x = x;
        coord.y = y;
        indexSemilla++;
        semillas[indexSemilla].p = true;
        semillas[indexSemilla].coord = coord;
    }

    // setear todos los meeseeks como inactivos
    for ( int player = 0; player < 2; player++){
        for (int i = 0; i < MAX_CANT_MEESEEKS; i++){
            meeseeks[player][i].p = 0;
        }
    }
}



void end_game(void){
    // breakpoint();
    //pantalla_negra_debug();
    print("FIN DEL JUEGO", 35, 5, C_FG_WHITE); // ver si quead centrado
}

void add_score(uint16_t player){
    breakpoint();
    score[player] += 425;
    print_dec(score[RICK], 8, 10, 43, WHITE_RED);
    print_dec(score[MORTY], 8, 62, 43, WHITE_BLUE);
}

_Bool same(coordenadas a, coordenadas b){
    return (a.x == b.x && a.y == b.y);
}

int8_t next_index_meeseek_free(int player){
    for (int8_t i = 0; i < MAX_CANT_MEESEEKS; i++){
        if (!meeseeks[player][i].p){
            return i;
        }   
    }
    return -1;         // 255 = no existe lugar libre para otro meeseek
}

    // busca si la coordenada esta sobre una semilla, ret -1 = no hay semilla en tal cordenada, ret x =  en semillas[x] esta la semilla encontrada
int8_t index_in_seed(coordenadas coord){        //! cambiar mañána que devuelva -1 si es false, y el index en caso de true asi queda mejor
    for (int i = 0; i < MAX_CANT_SEMILLAS; i++)
    {
        if (semillas[i].p){
            if ( same(coord, semillas[i].coord) ){
                // breakpoint();
                // print_dec(i, 8, 10, 8, WHITE_RED);
                return i;
            }
        }
    }
    return -1;
}

void bye_seed(int8_t idx){
    update_map_seed(semillas[idx].coord);
    semillas[idx].p = false; // chau semillas                 //! FALTA ACUTALIZAR EL MAPA(SACAR LA SEMILLA)
}

// ;
// in EAX = code Código de la tarea Mr Meeseeks a ser ejecutada.;
// in EBX = x Columna en el mapa donde crear el Mr Meeseeks.;
// in ECX = y Fila en el mapa donde crear el Mr Meeseeks

//code 4KB  Tener en cuenta que este código debe estar declarado dentro del espacio de memoria de usuario de la tarea.
uint32_t create_meeseek(uint32_t code, uint8_t x, uint8_t y ){
    
    int16_t player = player_idx_gdt[tareaActual];

    // filtro jugador                               VEER SI NO ES MEJOR FILTRAR ANTES
    if (player != RICK && player != MORTY){
        return 0;
    }

    // filtro coordenadas validas
    if (x > 80 || y > 39){
        return 0;
    }

    // filtramos que los meeseeks del jugador no esten en el limite de capacidad
    if (cant_meeseeks[player] == MAX_CANT_MEESEEKS){
        return 0;
    }

    coordenadas coord_actual;
    coord_actual.x = x;
    coord_actual.y = y;


    // si meeseek justo cae en semilla, suma puntos y chau semilla
    int8_t index_aux = index_in_seed(coord_actual);
    bool in_seed = index_aux != -1;
    // if(player == MORTY){
    //     in_seed = 1;
    // }

    //print_dec(in_seed, 8, 10, 7, WHITE_RED);

    if (in_seed){
        bye_seed(index_aux);
        add_score(player);
        return 0;
    }
    
    // crear Meeseek

    int8_t index_meeseek = next_index_meeseek_free(player);

    // mapear
    paddr_t virt_res;
    virt_res = tss_meeseeks_creator(player, index_meeseek + 1, code);

    meeseeks[player][index_meeseek].p = 1;
    meeseeks[player][index_meeseek].coord = coord_actual;
    update_meeseek_map(player, coord_actual, 1);  // 1 = ADD

    return virt_res;
}






















