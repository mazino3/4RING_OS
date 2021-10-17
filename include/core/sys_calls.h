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

/* Clear the sent IRQ from core scheduler queue buffer */
__inlinea_ void core_sys_clr_irq(u_char ptr) {
__asm{
	push ptr // Input arg. of irq which should be deleted from core scheduler
  	lcall CG_CORE_TX_IRQ,0 // System call thru the call gate desc. selector
    }
}

__inlinea_  u_int core_sys_tx_irq() {
__asm{
  push DECL_IRQ // Input arg to declare irq to core scheduler from lower rings
  lcall CG_CORE_TX_IRQ,0
  mov eax, dword ptr [esp] // This is return
  }
}

#endif /* __CORE_SYS_CALLS */
