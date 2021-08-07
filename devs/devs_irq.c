/*
 * 4RING_OS
 *
 * devs_irq.c
 *
 * Main interrupt handler for ring 1 is defined here
 * like a Task = devs_irq_task.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <gdt.h>
#include <sched.h>

extern int indx;					// table index for sched_req_struc
extern struct sched_req_struc dev_sched_tbl[];

extern void get_key_int(void);

void devs_irq_task(void);

/* Table of interrupt functions at devs ring 1.
 * Here is place to put all main routines of all devices
 * to handle their interrupts.
 */
void_func dev_int_ftbl[] = {	// devices interrupts functions table
0, get_key_int};

static void save_ptr(u_long ptr) {
  dev_sched_tbl[indx-1].req_ptr = ptr;
}

__naked void devs_irq_task(void) {
__asm{
	start:
	call [dev_int_ftbl + eax*4]	// sample > call [get_key_int] if eax=1
	push DECL_IRQ				// input argument, declare IRQ
	lcall CG_TX_IRQ,0			// call call gate selector sys call
	mov eax, dword ptr [esp]	// get result from returned call gate func
	push eax					// push arg on stack
	call save_ptr				// save given ptr from core_sched ptrs table
	add esp,0x04				// release arg from stack
	iretd						// return from dev irq task
	jmp short start				// task needs endless loop
  }
}

