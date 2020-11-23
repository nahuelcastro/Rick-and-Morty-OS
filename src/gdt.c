/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

	Definicion de la tabla de descriptores globales
*/

#include "gdt.h"



#define GDT_IDX_CODIGO_LVL_0 10
#define GDT_IDX_DATO_LVL_0 11
#define GDT_IDX_CODIGO_LVL_3 12
#define GDT_IDX_DATO_LVL_3 13
#define GDT_IDX_VIDEO_LVL_0 14

gdt_entry_t gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = /*(gdt_entry_t)*/ {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x0000,
            .limit_19_16 = 0x00,
            .type = 0x0,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
        },

// segmento de codigo nivel 0
    [GDT_IDX_CODIGO_LVL_0] = /*(gdt_entry_t)*/{
        .base_15_0 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_15_0 = 0xc8ff,
        .limit_19_16 = 0x00,
        .type = 0xa,
        .s = 0x01,
        .dpl = 0x00,
        .p = 0x01,
        .avl = 0x0,
        .l = 0x0,
        .db = 0x1,
        .g = 0x01,
    },

// segmento de datos nivel 0
    [GDT_IDX_DATO_LVL_0] = /*(gdt_entry_t)*/{
        .base_15_0 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_15_0 = 0xc8ff,
        .limit_19_16 = 0x00,
        .type = 0x2,
        .s = 0x01,
        .dpl = 0x00,
        .p = 0x01,
        .avl = 0x0,
        .l = 0x0,
        .db = 0x1,
        .g = 0x01,
    },

// segmento de codigo nivel 3
    [GDT_IDX_CODIGO_LVL_3] = /*(gdt_entry_t)*/{
        .base_15_0 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_15_0 = 0xc8ff,
        .limit_19_16 = 0x00,
        .type = 0xa,
        .s = 0x01,
        .dpl = 0x03,
        .p = 0x01,
        .avl = 0x0,
        .l = 0x0,
        .db = 0x1,
        .g = 0x01,
    },

// segmento de datos nivel 3
    [GDT_IDX_DATO_LVL_3] = /*(gdt_entry_t)*/{
        .base_15_0 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_15_0 = 0xc8ff,
        .limit_19_16 = 0x00,
        .type = 0x2,
        .s = 0x01,
        .dpl = 0x03,
        .p = 0x01,
        .avl = 0x0,
        .l = 0x0,
        .db = 0x1,
        .g = 0x01,
    },
// segmento de pantalla  nivel 0
    [GDT_IDX_VIDEO_LVL_0] = /*(gdt_entry_t)*/{
        .base_15_0 = 0x8000,
        .base_23_16 = 0x0B,
        .base_31_24 = 0x00,
        .limit_15_0 = 0x1f3f,   // 80x50(tamaño de panatalla) * 2 (1 byte para lo que queres imprimir y otro formato) = 8000d
        .limit_19_16 = 0x00,
        .type = 0x2,
        .s = 0x01,
        .dpl = 0x00,
        .p = 0x01,
        .avl = 0x0,
        .l = 0x0,
        .db = 0x1,
        .g = 0x00,
    },
};

gdt_descriptor_t GDT_DESC = {
  sizeof(gdt) - 1,
  (uint32_t) &gdt
};
