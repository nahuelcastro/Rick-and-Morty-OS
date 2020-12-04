/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"

uint16_t ultimoJugador = 0;
uint16_t jugadorActual = 1;
uint16_t index;
uint16_t tareaActual;
tss_t* tssActual;

extern void pantalla_negra_debug();
extern void init_pantalla();

bool modoDebug;

void sched_init(void)
{
  index = 16;
  tareaActual = index;
  modoDebug = false;
}

uint16_t sched_next_task(void){

  // breakpoint();

  for (int i = 0; i < GDT_COUNT - 1; i++)
  { /// TOCAR EL I
    if (index == GDT_COUNT)
    {
      index = 15;
    }

    bool esUnJugador = player_idx_gdt[index + 1] == RICK || player_idx_gdt[index + 1] == MORTY;
    bool esOtroJugador = (player_idx_gdt[index + 1] != player_idx_gdt[tareaActual]) && esUnJugador;
    bool estaPresente = gdt[index + 1].p == 1;
    bool estaActiva = tareasActivas[index + 1];

    if (estaPresente && esOtroJugador && estaActiva)
    {
      index++;
      tareaActual = index;
      return (index << 3); //! ver si hace falta sumarle 3 por los niveles de privilegio
    }
    else
    {
      index++;
    }
  }
  tssActual = TSSs[tareaActual];
  return (tareaActual << 3);
}

void desactivar_tarea(){
  if(tareaActual == 17 || tareaActual == 18){
    end_game();
  }

  tareasActivas[tareaActual] = false;
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