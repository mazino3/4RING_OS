/*
 * 4RING_OS License: MIT
 *
 * core/sys_calls.h
 *
 * System call functions to the core (ring 0) thru the call gates.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef __CORE_SYS_CALLS
#define __CORE_SYS_CALLS

#include <typedef.h>
#include <gdt.h>
#include <sched.h>

/* Clear the sent IRQ from core scheduler queue buffer */
static __inl_unusd_ void core_sys_clr_irq(u_char ptr)
{
  __asm{
	push ptr // Input arg. of irq which should be deleted from core scheduler
	lcall CG_CORE_TX_IRQ,0 // System call thru the call gate desc. selector
  }
}

static __inl_unusd_  u_int core_sys_tx_irq(void)
{
  __asm{
	push DECL_IRQ // Input arg to declare irq to core scheduler from lower rings
	lcall CG_CORE_TX_IRQ,0
	mov eax, dword ptr [esp] // This is return
  }
}

/*
 * Function printr (print ring) is a sys call.
 * Has an input arg ptr = start of stack where is arguments
 */
static __inl_unusd_ int __printr(void *ptr)
{
  __asm{
	push ptr
	lcall CG_CORE_PRINTR,0
  }
}

#endif /* __CORE_SYS_CALLS */
