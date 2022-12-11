/*
 * 4RING_OS License: MIT
 *
 * gdt_mem.c
 *
 * Setup global descriptor table at core (ring 0).
 * For now table is statically allocated.
 *
 * It is planned that all 8192 descriptors will be used.
 * That's why is given the GDT table a special memory location at
 * the end of the available memory. By statically limiting of the rings
 * memory spaces, only ring 0 will be able to access this location.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <sys.h>

__attribute__((section(".gdt_data")))
l_long gdt[ENTR_SIZE] = {
	0x0000000000000000, // NULL descriptor, DUMMY
	/// CODE & DATA descriptors for general 4 RINGS
	// RING 0
	0x00c09a0000000000 + MEM_LIMIT,		// CORE_CODE sel. 0x08 ring 0
	0x00c0920000000000 + MEM_LIMIT,		// CORE_DATA sel. 0x10 ring 0
	// RING 1
	0x00c0ba0000000000 + DEVS_LIMIT,	// DEVS_CODE sel. 0x18 ring 1
	0x00c0b20000000000 + DEVS_LIMIT,	// DEVS_DATA sel. 0x20 ring 1
	// RING 2
	0x00c0da0000000000 + LIBS_LIMIT,	// LIBS_CODE sel. 0x28 ring 2
	0x00c0d20000000000 + LIBS_LIMIT,	// LIBS_DATA sel. 0x30 ring 2
	// RING 3
	0x00c0fa0000000000 + USERS_SYS_LIMIT, // USERS_CODE sel. 0x38 ring 3
	0x00c0f20000000000 + USERS_SYS_LIMIT, // USERS_DATA sel. 0x40 ring 3
	/// System segment descriptors
	// RING 0 TASK
	0x0000890000000400, // TSS_CORE  sel. 0x48
	0x0000820000000400, // LDT_CORE  sel. 0x50
	// RING 1 TASKS
	0x0000a90000000400, // TSS_DEVS_IRQ   sel. 0x58
	0x0000a20000000400, // LDT_DEVS_IRQ   sel. 0x60
	0x0000a90000000400, // TSS_DEVS_SCHED sel. 0x68
	0x0000a20000000400, // LDT_DEVS_SCHED sel. 0x70
	// RING 2 TASKS (for now undefined)
	0x0000C90000000400, // TSS_LIBS_IRQ   sel. 0x78
	0x0000C20000000400, // LDT_LIBS_IRQ   sel. 0x80
	0x0000000000000000, // TSS_LIBS_SCHED sel. 0x88
	0x0000000000000000, // LDT_LIBS_SCHED sel. 0x90
	// RING 3 TASKS (for now undefined)
	0x0000e90000000400, // TSS_MAIN_TASK  sel. 0x98
	0x0000e20000000400, // LDT_MAIN_TASK  sel. 0xA0
	0x0000000000000000, // TSS_USERS_IRQ  sel. 0xA8
	0x0000000000000000, // LDT_USERS_IRQ  sel. 0xB0
	//0x0000000000000000, // TSS_USERS_SCHED 0xA8
	//0x0000000000000000, // LDT_USERS_SCHED 0xB0
	// RESERVED
	0x0000000000000000, //  sel. 0xB8
	0x0000000000000000, //  sel. 0xC0
	0x0000000000000000, //  sel. 0xC8
	0x0000000000000000, //  sel. 0xD0
	0x0000000000000000, //  sel. 0xD8
	0x0000000000000000, //  sel. 0xE0
	0x0000000000000000, //  sel. 0xE8
	/// Data access trough segment registers
	0x00c0f20000000000 + DEVS_LIMIT, //  USRS_ACCES_DATA sel. 0xF0
	// To access interupts from users to devs.
	0x00C0B200000007ff, //  DEVS_ACCES_DATA sel. 0xF8; used for GS register
	/// CALL GATES
	0x0000ec0100080000, // CG_CORE_TX_IRQ sel. 0x100 decs. for RING 0
	0x0000ec0100080000, // CG_CORE_PRINTR sel. 0x108 decs. for RING 0
	0x0000ec01002A0000, // CG_LIBS_TX_IRQ sel. 0x110 decs. for RING 2
	0x0000cc03001A0000, // CG_DEVS_TTY_W  sel. 0x118 decs. for RING 1 for rpl 2
};
