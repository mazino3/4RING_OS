/*
 * 4RING_OS License: MIT
 *
 * 4libc/functs_tbl.c
 *
 * Represents at memory table of standard libc functions.
 * Here is place to put all functions of 4libc library.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>

extern void _printf_(void);

/* Libc functions table */
void_func libc_ftbl[] = {
	  0, _printf_
};
