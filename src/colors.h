/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de constantes de con colores para la interfaz en modo texto.
*/

#ifndef __COLORS_H__
#define __COLORS_H__

/* Definicion de colores */
/* -------------------------------------------------------------------------- */
#define C_FG_BLACK         (0x0)
#define C_FG_BLUE          (0x1)
#define C_FG_GREEN         (0x2)
#define C_FG_CYAN          (0x3)
#define C_FG_RED           (0x4)
#define C_FG_MAGENTA       (0x5)
#define C_FG_BROWN         (0x6)
#define C_FG_LIGHT_GREY    (0x7)
#define C_FG_DARK_GREY     (0x8)
#define C_FG_LIGHT_BLUE    (0x9)
#define C_FG_LIGHT_GREEN   (0xA)
#define C_FG_LIGHT_CYAN    (0xB)
#define C_FG_LIGHT_RED     (0xC)
#define C_FG_LIGHT_MAGENTA (0xD)
#define C_FG_LIGHT_BROWN   (0xE)
#define C_FG_WHITE         (0xF)

#define C_BG_BLACK      (0x0 << 4)
#define C_BG_BLUE       (0x1 << 4)
#define C_BG_GREEN      (0x2 << 4)
#define C_BG_CYAN       (0x3 << 4)
#define C_BG_RED        (0x4 << 4)
#define C_BG_MAGENTA    (0x5 << 4)
#define C_BG_BROWN      (0x6 << 4)
#define C_BG_LIGHT_GREY (0x7 << 4)


#define C_BLINK (0x8 << 4)

// primero color fuente y luego color fondo
#define WHITE_RED 0x4f
#define WHITE_BLACK 0x0f
#define WHITE_BLUE 0x1f
#define BLACK_RED 0x04
#define BLACK_BLUE 0x01
#define BLACK_WHITE 0x0f
#define GREEN_YELLOW 0x2E

#define BLUE_GREEN 0x21
#define RED_GREEN 0x24
#define GREEN_GREEN 0x22
#define BLACK_BLACK 0x00

#define RICK_MEESEEK_COLOR RED_GREEN
#define MORTY_MEESEEK_COLOR BLUE_GREEN

#endif //  __COLORS_H__

/*
bochs 

./configure --enable-gdb-stub --enable-disasm --disable-docbook --enable-readline LDFLAGS='-pthread' --prefix=/home/nahuel/bochs-gdb/bochs/



*/
