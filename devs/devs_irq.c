/*
 * 4RING_OS License: MIT
 *
 * devs_irq.c
 *
 * Main interrupt handler for ring 1 is defined here
 * like a Task = devs_irq_task.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <gdt.h>
#include <task.h>
#include <sched.h>
#include <core/sys_calls.h>
#include <string.h>

extern int indx; /* Table index for sched_req_struc */
extern struct sched_req_struc dev_sched_tbl[];
extern struct tss_struct tss_devs_irq;

extern void get_key_int(void);

void devs_irq_task(void);


/* Table of interrupt functions at devs ring 1.
 * Here is place to put all main routines of all devices
 * to handle their interrupts.
 */
void_func dev_int_ftbl[] = { /* Devices interrupts functions table */
0, get_key_int};

static __inlinea_ void save_ptr(u_int ptr) {
  dev_sched_tbl[indx-1].req_ptr = ptr;
}

/* This is main task function for devs IRQ's at ring 1 */
void devs_irq_task(void) {

  for (;;) { /* This is important for the Task, like a unusual function */
	  dev_int_ftbl[tss_devs_irq.eax](); // Call function from table index
	  save_ptr(core_sys_tx_irq()); // Send irq to core and save returned ptr
	  int_ret(); // Return from interrupt
  }   /* endless loop for task */
}


