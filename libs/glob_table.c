/*
 * 4RING_OS License: MIT
 *
 * libs/glob_table.c
 *
 * Global table(s) for descriptors of all libs loaded at libs mem space.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <libs/lib_defs.h>
#include <typedef.h>

extern void libc_ftbl(void);

/* indexed table of all libs */
void_func libs_ftbl[] = {
	0,		   // LIB_SYS
	libc_ftbl, // LIBC
	0,		   // LIBX
};
