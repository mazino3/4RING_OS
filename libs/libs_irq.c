/*
 * 4RING_OS License: MIT
 *
 * libs_irq.c
 *
 * Main interrupt handler for ring 2 is defined here
 * like a Task = libs_irq_task.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <gdt.h>
#include <ldt.h>
#include <task.h>
#include <sched.h>
#include <io.h>
#include <core/sys_calls.h>
#include <libs/lib_sys.h>

void libs_irq_task(void);

extern void_func libs_ftbl[];
extern struct tss_struct tss_libs_irq;

/* This is main task function for libs IRQ's at ring 2 */
void libs_irq_task(void) { // Interrupt is off by default for while...

  u_int register major, minor;
  /* Alert from first time initialization */
  printr("Hello from libs (ring 2)!! \r\n");
  for (;;) {
	  /*decode indexed lib(major) and func table in it(minor) from input arg */
	  major = tss_libs_irq.ebx; // get coded input arg from tss-structure
	  minor = (major & 0xFFFF); // extract major and minor
	  /* Call decoded minor = lib function (like printf) from decoded major =
	   * library (like libc) from indexed function tables. */
	  ((void_func*)libs_ftbl[major>>16])[minor]();
	  int_ret();
  }
}

/* This function (sys call to libraries) is called only from user space like
 * interrupt request for libs and their functions. Main argument is in eax
 * register and is a major-minor coded integer for calling some library and
 * their function over the call gate. Other input arguments is send thru stack
 * as a single pointer to set or structure of arguments.
 */
__naked_ void libs_irq(void){
  __asm{
	push ebx
	mov ebx, LIBS_DATA         // adjust ds to ring 2
	mov ds, bx
	mov tss_libs_irq.ebx, eax  // save coded input argument at tss-structure.
	mov eax, [esp+12]
	mov tss_libs_irq.edx, eax  // save the stack where is input  arguments
	// jump to ring 2 main irq function defined in GDT descriptor
	lcall TSS_LIBS_IRQ,0       // enter libs_irq_task (switch from ring 3 to 2)!
	// prepare return from call gate
	mov eax, tss_libs_irq.eax  // return arg
  	mov ebx, TASK_DATA         // adjust ds to the user space ring 3
  	mov ds, bx
	pop ebx
  	retf 4
  }
}


