/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Rutinas del controlador de interrupciones.
*/
#include "pic.h"

#define PIC1_PORT 0x20
#define PIC2_PORT 0xA0

static __inline __attribute__((always_inline)) void outb(uint32_t port,
                                                         uint8_t data) {
  __asm __volatile("outb %0,%w1" : : "a"(data), "d"(port));
}
void pic_finish1(void) { outb(0x20, 0x20); }
void pic_finish2(void) {
  outb(0x20, 0x20);
  outb(0xA0, 0x20);
}

void pic_reset() {
  outb(PIC1_PORT + 0, 0x11); /* IRQs activas x flanco, cascada, y ICW4 */
  outb(PIC1_PORT + 1, 0x20); /* Addr */
  outb(PIC1_PORT + 1, 0x04); /* PIC1 Master, Slave ingresa Int.x IRQ2 */
  outb(PIC1_PORT + 1, 0x01); /* Modo 8086 */
  outb(PIC1_PORT + 1, 0xFF); /* Enmasca todas! */
  outb(PIC2_PORT + 0, 0x11); /* IRQs activas x flanco, cascada, y ICW4 */
  outb(PIC2_PORT + 1, 0x28); /* Addr */
  outb(PIC2_PORT + 1, 0x02); /* PIC2 Slave, ingresa Int x IRQ2 */
  outb(PIC2_PORT + 1, 0x01); /* Modo 8086 */
  outb(PIC2_PORT + 1, 0xFF); /* Enmasca todas! */
}

void pic_enable() {
  outb(PIC1_PORT + 1, 0x00);
  outb(PIC2_PORT + 1, 0x00);
}

void pic_disable() {
  outb(PIC1_PORT + 1, 0xFF);
  outb(PIC2_PORT + 1, 0xFF);
}
