/*
 * 4RING_OS License: MIT
 *
 * 4libc/libc_calls.h
 *
 * Libc call functions to the libs (ring 2) thru the call gates.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef __LIBC_CALLS
#define __LIBC_CALLS

#include <typedef.h>
#include <gdt.h>
#include <sched.h>
#include <libs/lib_defs.h>

/* minor defines for libc */
#define _PRINTF 1

#define PRINTF ((MAJOR(LIBC)) | (MINOR(_PRINTF)))

/*
 * Function __printf (print formated) is a sys call.
 * It uses the inline assembler code to call the function of the same
 * name (actually _printf_) into libs (ring 2) through the call gate.
 */
static __inl_unusd_ int __printf(void *ptr)
{
  __asm{
	mov eax, PRINTF
	push ptr
	lcall CG_LIBS_TX_IRQ,0
  }
}

#endif /* __LIBC_CALLS */
