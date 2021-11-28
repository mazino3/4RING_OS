/*
 * 4RING_OS License: MIT
 *
 * devs/devs_calls.h
 *
 * System call functions to the devs (ring 1) thru the call gates.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef __DEVS_SYS_CALLS
#define __DEVS_SYS_CALLS

#include <typedef.h>
#include <gdt.h>
#include <sched.h>

/*
 * Function tty_write is a sys call. The function with same name
 * exist in ring 1 at devs/tty.c file, but they work without interfering
 * with each other.
 * Has an 3 input args = ...
 */
static __inl_unusd_ int tty_write(int chan, char *buf, int nr)
{
  __asm{
	push nr
	push buf
	push chan
	lcall CG_DEVS_TTY_W,0
  }
}

#endif /* __DEVS_SYS_CALLS */


