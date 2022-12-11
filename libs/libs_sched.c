/*
 * 4RING_OS
 *
 * libs_sched.c
 *
 * Here is defined libs_sched_task
 * that schedules various libs functions.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <task.h>
#include <sched.h>
#include <core/sys_calls.h>
#include <libs/lib_sched.h>
#include <string.h>

extern struct tss_struct tss_libs_irq;

#define TBL_SIZE 0xFF

int i_lib = 0;				// table index

/* Structure for circular schedule buffer */
struct libs_sched_struct lib_sched_tbl[TBL_SIZE] = {0};

/* This is a Task function, a main scheduler for libs - ring 1 */
void libs_sched_task(void) {
	u_int val;
	int i = 0;
	for (;;) {	// This is important for Task, like a unusual function
		while (lib_sched_tbl[i].func){
			val = lib_sched_tbl[i].value;
			val = lib_sched_tbl[i].func(val); // call procedure to complete irq
			tss_libs_irq.eax = val; // save return
			lib_sched_tbl[i].value = 0; // erase irq from libs circular table buff
			lib_sched_tbl[i].func = 0;
			core_sys_clr_irq(lib_sched_tbl[i].req_ptr);
			i++;
			if (i == TBL_SIZE)
				i = 0;
		}
	}	/* endless loop for task, TODO: try here force_sched();*/
	//force_sched();
}

/* Save irq to local circular buffer (lib_sched_tbl)
void save_lib_irq(u_int val, void_f_int func) {
  lib_sched_tbl[i_lib].value = val;
  lib_sched_tbl[i_lib].func = func;
  i_lib ++;
  if (i_lib == TBL_SIZE)
	i_lib = 0;
}
*/
