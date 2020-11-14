/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

	Definicion de la tabla de descriptores globales
*/

#include "gdt.h"

#define GDT_IDX_DATO_LVL_0 10
#define GDT_IDX_DATO_LVL_3 11
#define GDT_IDX_CODIGO_LVL_0 12
#define GDT_IDX_CODIGO_LVL_3 13
#define GDT_IDX_VIDEO_LVL_0 14


gdt_entry_t gdt[GDT_COUNT] = {
		/* Descriptor nulo*/
		/* Offset = 0x00 */
		[GDT_IDX_NULL_DESC] =
		{
						.limit_15_0 = 0x0000,
						.base_15_0 = 0x0000,
						.base_23_16 = 0x00,
						.type = 0x0,
						.s = 0x00,
						.dpl = 0x00,
						.p = 0x00,
						.limit_19_16 = 0x00,
						.avl = 0x0,
						.l = 0x0,
						.db = 0x0,
						.g = 0x0,
						.base_31_24 = 0x00,
				},
				[GDT_IDX_DATO_LVL_0] ={
					.base_15_0 = 0,
					.base_23_16 = 0,
					.base_31_24 = 0,
					.limit_15_0 = 0x1455,
					.limit_19_16 = 0x5,
					.type = 2, //2: Read/Write
					.s = 1, //1: Código/Datos
					.dpl = 0,
					.p = 1, // Bit de "Present"
					.avl = 0,
					.l = 0,
					.db = 1, // 1: 32 bits, 0: 16 bits
					.g = 1,
				},
				[GDT_IDX_DATO_LVL_3] ={
					.base_15_0 = 0,
					.base_23_16 = 0,
					.base_31_24 = 0,
					.limit_15_0 = 0x1455,
					.limit_19_16 = 0x5,
					.type = 2, //2: Read/Write
					.s = 1, //1: Código/Datos
					.dpl = 0,
					.p = 1, // Bit de "Present"
					.avl = 0,
					.l = 0,
					.db = 1, // 1: 32 bits, 0: 16 bits
					.g = 1,
				},
				[GDT_IDX_CODIGO_LVL_0] ={
					.base_15_0 = 0,
					.base_23_16 = 0,
					.base_31_24 = 0,
					.limit_15_0 = 0x1455,
					.limit_19_16 = 0x5,
					.type = 0xa, //10: Execute/Read
					.s = 1, //1: Código/Datos
					.dpl = 0,
					.p = 1, // Bit de "Present"
					.avl = 0,
					.l = 0,
					.db = 1, // 1: 32 bits, 0: 16 bits
					.g = 1,
				},
				[GDT_IDX_CODIGO_LVL_3] ={
					.base_15_0 = 0,
					.base_23_16 = 0,
					.base_31_24 = 0,
					.limit_15_0 = 0x1455,
					.limit_19_16 = 0x5,
					.type = 0xa, //10: Execute/Read
					.s = 1, //1: Código/Datos
					.dpl = 0,
					.p = 1, // Bit de "Present"
					.avl = 0,
					.l = 0,
					.db = 1, // 1: 32 bits, 0: 16 bits
					.g = 1,
				},
				[GDT_IDX_VIDEO_LVL_0] ={
					.base_15_0 = 0x8000,
					.base_23_16 = 0x0b,
					.base_31_24 = 0,
					.limit_15_0 = 0x1f3f,
					.limit_19_16 = 0,
					.type = 0xa, //10: Execute/Read
					.s = 1, //1: Código/Datos
					.dpl = 0,
					.p = 1, // Bit de "Present"
					.avl = 0,
					.l = 0,
					.db = 1, // 1: 32 bits, 0: 16 bits
					.g = 1,
				},
};

gdt_descriptor_t GDT_DESC = {sizeof(gdt) - 1, (uint32_t)&gdt};
