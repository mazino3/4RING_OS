/*
 * 4RING_OS License: MIT
 *
 * gdt.c
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
						
struct gdt_opcode_p {
	u_short size;		// GDT_MAX_ENT entries
	u_int * gdt;		// ptr of gdt table + core base
} gdt_opcode = { GDT_ENTRIES, ((u_int *) & gdt[0]) + CORE_BASE };

__naked void setup_gdt(void) {
__asm{
	lgdt gdt_opcode
	ret
	}
}
