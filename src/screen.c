/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "screen.h"

void print_scan_code(uint8_t scancode){
  if( 0x1 < scancode && scancode < 0xc ){
      print_hex(scancode-1, 2, 78,0 , 0x3D);
  }
  else if(!( scancode & 0x80)){
      print_hex(scancode, 2, 80/2-1, 50/2, 0x3D);
  }
}

void print_libretas(){
  print("203/19, 248/19, 310/19 ", 0, 0, 0x0A);
}



void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  for (i = 0; text[i] != 0; i++) {
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS) {
      x = 0;
      y++;
    }
  }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  uint8_t letras[16] = "0123456789";

  for (i = 0; i < size; i++) {
    uint32_t resto = numero % 10;
    numero = numero / 10;
    p[y][x + size - i - 1].c = letras[resto];
    p[y][x + size - i - 1].a = attr;
  }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  uint8_t hexa[8];
  uint8_t letras[16] = "0123456789ABCDEF";
  hexa[0] = letras[(numero & 0x0000000F) >> 0];
  hexa[1] = letras[(numero & 0x000000F0) >> 4];
  hexa[2] = letras[(numero & 0x00000F00) >> 8];
  hexa[3] = letras[(numero & 0x0000F000) >> 12];
  hexa[4] = letras[(numero & 0x000F0000) >> 16];
  hexa[5] = letras[(numero & 0x00F00000) >> 20];
  hexa[6] = letras[(numero & 0x0F000000) >> 24];
  hexa[7] = letras[(numero & 0xF0000000) >> 28];
  for (i = 0; i < size; i++) {
    p[y][x + size - i - 1].c = hexa[i];
    p[y][x + size - i - 1].a = attr;
  }
}

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = fInit; f < fInit + fSize; f++) {
    for (c = cInit; c < cInit + cSize; c++) {
      p[f][c].c = character;
      p[f][c].a = attr;
    }
  }
}
















///


// uint16_t ancho_pantalla = 80;
// uint16_t alto_pantalla = 50;
// uint32_t modo_debug = 0;

// void BSOD(uint32_t valor_excepcion, uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi, uint32_t ebp, uint32_t esp, uint32_t eip, uint32_t cs, uint32_t ds, uint32_t es, uint32_t fs, uint32_t gs, uint32_t ss, uint32_t cr0, uint32_t cr1, uint32_t cr2, uint32_t cr3, uint32_t cr4, uint32_t eflags)
// {

//   modo_debug = 2;
//   uint16_t *posicion_pantalla;

//   int ancho_stack = 14;
//   int ancho = 40;
//   int alto = 28;
//   int x_base = (ancho_pantalla - ancho - ancho_stack) / 2 - 2;
//   int y_base = (alto_pantalla - alto) / 2;

//   //Sombra caja registros
//   for (int y = 0; y < alto + 2; y++)
//   {
//     for (int x = 0; x < ancho + 2; x++)
//     {
//       posicion_pantalla = (uint16_t *)0x0B8000 + ((y + y_base - 1) * ancho_pantalla) + x + x_base - 1;
//       uint16_t atributos = (3 << 4) | (0xF);
//       *posicion_pantalla = (' ') | (atributos << 8);
//     }
//   }

//   //Caja registros
//   for (int y = 0; y < alto; y++)
//   {
//     for (int x = 0; x < ancho; x++)
//     {
//       posicion_pantalla = (uint16_t *)0x0B8000 + ((y + y_base) * ancho_pantalla) + x + x_base;
//       uint16_t atributos = (7 << 4) | (0xF);
//       *posicion_pantalla = (' ') | (atributos << 8);
//     }
//   }

//   //Sombra caja stack
//   for (int y = 0; y < alto + 2; y++)
//   {
//     for (int x = 0; x < ancho_stack + 2; x++)
//     {
//       posicion_pantalla = (uint16_t *)0x0B8000 + ((y + y_base - 1) * ancho_pantalla) + x + x_base + ancho + 2;
//       uint16_t atributos = (3 << 4) | (0xF);
//       *posicion_pantalla = (' ') | (atributos << 8);
//     }
//   }

//   //Caja stack
//   for (int y = 0; y < alto; y++)
//   {
//     for (int x = 0; x < ancho_stack; x++)
//     {
//       posicion_pantalla = (uint16_t *)0x0B8000 + ((y + y_base) * ancho_pantalla) + x + x_base + ancho + 3;
//       uint16_t atributos = (7 << 4) | (0xF);
//       *posicion_pantalla = (' ') | (atributos << 8);
//     }
//   }

//   char *nombre_excepcion = "?? - Desconocida";
//   int cant_excepciones = 21;
//   char *excepciones[] = {"0x0 - Divide-by-zero Error", "0x1 - Debug", "0x2 - Non-maskable Interrupt", "0x3 - Breakpoint", "0x4 - Overflow", "0x5 - Bound Range Exceeded", "0x6 - Invalid Opcode", "0x7 - Device Not Available", "0x8 - Double Fault", "0x9 - Coprocessor Segment Overrun", "0xA - Invalid TSS", "0xB - Segment Not Present", "0xC - Stack-Segment Fault", "0xD - General Protection Fault", "0xE - Page Fault", "0xF - (Reserved)", "0x10 - x87 Floating-Point Exception", "0x11 - Alignment Check", "0x12 - Machine Check", "0x13 - SIMD Floating-Point Exception", "0x14 - Virtualization Exception"};

//   if (valor_excepcion < cant_excepciones)
//     nombre_excepcion = excepciones[valor_excepcion];

//   poner_string("Saltadores y Cazadores", x_base + 1, y_base + 1, 7, 0xF);
//   //poner_string(error, 10, 18, color_bsod, 0xF);
//   poner_string("Excepcion:", x_base + 1, y_base + 3, 7, 0);
//   poner_string(nombre_excepcion, x_base + 1, y_base + 4, 7, 0x1);

//   poner_string("EAX:", x_base + 1, y_base + 6, 7, 0);
//   poner_hex(eax, x_base + 6, y_base + 6, 7, 0x1);
//   poner_string("EBX:", x_base + 2 + (ancho / 2), y_base + 6, 7, 0);
//   poner_hex(ebx, x_base + 2 + (ancho / 2) + 5, y_base + 6, 7, 0x1);

//   poner_string("ECX:", x_base + 1, y_base + 8, 7, 0);
//   poner_hex(ecx, x_base + 6, y_base + 8, 7, 0x1);
//   poner_string("EDX:", x_base + 2 + (ancho / 2), y_base + 8, 7, 0);
//   poner_hex(edx, x_base + 2 + (ancho / 2) + 5, y_base + 8, 7, 0x1);

//   poner_string("ESI:", x_base + 1, y_base + 10, 7, 0);
//   poner_hex(esi, x_base + 6, y_base + 10, 7, 0x1);
//   poner_string("EDI:", x_base + 2 + (ancho / 2), y_base + 10, 7, 0);
//   poner_hex(edi, x_base + 2 + (ancho / 2) + 5, y_base + 10, 7, 0x1);

//   poner_string("ESP:", x_base + 1, y_base + 12, 7, 0);
//   poner_hex(esp, x_base + 6, y_base + 12, 7, 0x1);
//   poner_string("EIP:", x_base + 2 + (ancho / 2), y_base + 12, 7, 0);
//   poner_hex(eip, x_base + 2 + (ancho / 2) + 5, y_base + 12, 7, 0x1);

//   poner_string("CS:", x_base + 1, y_base + 14, 7, 0);
//   poner_hex(cs, x_base + 6, y_base + 14, 7, 0x1);
//   poner_string("DS:", x_base + 2 + (ancho / 2), y_base + 14, 7, 0);
//   poner_hex(ds, x_base + 2 + (ancho / 2) + 5, y_base + 14, 7, 0x1);

//   poner_string("ES:", x_base + 1, y_base + 16, 7, 0);
//   poner_hex(es, x_base + 6, y_base + 16, 7, 0x1);
//   poner_string("FS:", x_base + 2 + (ancho / 2), y_base + 16, 7, 0);
//   poner_hex(fs, x_base + 2 + (ancho / 2) + 5, y_base + 16, 7, 0x1);

//   poner_string("GS:", x_base + 1, y_base + 18, 7, 0);
//   poner_hex(gs, x_base + 6, y_base + 18, 7, 0x1);
//   poner_string("SS:", x_base + 2 + (ancho / 2), y_base + 18, 7, 0);
//   poner_hex(ss, x_base + 2 + (ancho / 2) + 5, y_base + 18, 7, 0x1);

//   poner_string("CR0:", x_base + 1, y_base + 20, 7, 0);
//   poner_hex(cr0, x_base + 6, y_base + 20, 7, 0x1);
//   poner_string("CR1:", x_base + 2 + (ancho / 2), y_base + 20, 7, 0);
//   poner_hex(cr1, x_base + 2 + (ancho / 2) + 5, y_base + 20, 7, 0x1);

//   poner_string("CR2:", x_base + 1, y_base + 22, 7, 0);
//   poner_hex(cr2, x_base + 6, y_base + 22, 7, 0x1);
//   poner_string("CR3:", x_base + 2 + (ancho / 2), y_base + 22, 7, 0);
//   poner_hex(cr3, x_base + 2 + (ancho / 2) + 5, y_base + 22, 7, 0x1);

//   poner_string("CR4:", x_base + 1, y_base + 24, 7, 0);
//   poner_hex(cr4, x_base + 6, y_base + 24, 7, 0x1);
//   poner_string("EFLAGS:", x_base + 2 + (ancho / 2), y_base + 24, 7, 0);
//   poner_hex(eflags, x_base + 2 + (ancho / 2) + 5, y_base + 26, 7, 0x1);

//   poner_string("EBP:", x_base + 1, y_base + 26, 7, 0);
//   poner_hex(ebp, x_base + 6, y_base + 26, 7, 0x1);

//   poner_string("Stack:", x_base + ancho + 3 + 1, y_base + 1, 7, 0);
//   poner_string("(desde tope)", x_base + ancho + 3 + 1, y_base + 2, 7, 0);
//   uint32_t *valor_stack = (uint32_t *)(esp + 22 * 4);
//   for (int a = 0; a < alto - 5; a++)
//   {
//     if (esp >= ebp)
//       break;
//     poner_hex(*valor_stack, x_base + ancho + 3 + 1, y_base + 4 + a, 7, 0x1);
//     ++valor_stack;
//   }
// }
