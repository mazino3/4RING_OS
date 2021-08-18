/*
 * 4RING_OS
 *
 * devs_sched.c
 *
 * Here is defined devs_sched_task
 * that schedules various devs functions.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <sched.h>
//#include <gates.h>		// for the call_gates

#define TBL_SIZE 0xFF

int indx = 0;				// table index

/* Structure for circular schedule buffer */
struct sched_req_struc dev_sched_tbl[TBL_SIZE] = {0};

/* Clear irq pointer from core scheduler circular table buffer */
__inline_ static void clr_core_irq(u_char ptr) {
__asm{
	push ptr
	lcall CG_TX_IRQ,0		// system call with call gate desc. selector
  }
}

/* This is a Task function, a main scheduler for devs - ring 1 */
void devs_sched_task(void) {
  u_int val;
  int i = 0;
  for (;;) {	// This is important for Task, like a unusual function
	  while (dev_sched_tbl[i].func){
	  	val = dev_sched_tbl[i].value;
	  	dev_sched_tbl[i].func(val); // call procedure to complete irq
	  	dev_sched_tbl[i].value = 0; // erase irq from devs circular table buff
	  	dev_sched_tbl[i].func = 0;
	  	clr_core_irq(dev_sched_tbl[i].req_ptr);
	  	i++;
	  	if (i == TBL_SIZE)
	  	  i = 0;
	  }
  }   // endless loop for task
}

/* Save irq to local circular buffer (dev_sched_tbl) */
void save_irq(u_int val, void_f_int func) {
  dev_sched_tbl[indx].value = val;
  dev_sched_tbl[indx].func = func;
  indx ++;
  if (indx == TBL_SIZE)
	indx = 0;
}

