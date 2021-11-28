/*
 * 4RING_OS
 *
 * print_sys.c
 *
 * There is defined functions for printing system message at console from wide
 * system etc from all rings.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <ldt.h>
#include <sched.h>
#include <stdarg.h>
#include <rings.h>
#include <io.h>

#include <task.h>
#include <sys.h>

static char buf[1024];

void printr_task(void);
extern void con_printr(int ring, const char * buf);
extern int vsprintf (char * buf, const char * fmt, va_list args );

/*
 * Function printc (print core) is replacement for printk from UNIX-like kernels.
 * It prints in console from ring 0 (core).
 */
int printc(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsprintf(buf, fmt, args);
	va_end(args);
	con_printr(CORE, buf); // Called from ring 0 to ring 1 ... no problem.
	return i;
}

/*
 * Function printr (print ring) is actually from a sys call thru call gate from
 * lower rings than 0 (core). First it must prepared to be called by call gate
 * from 'lower' ring by _printr function... There 'lower' means by priority.
 */
int printr(int ring, const char *fmt, void *ptr)
{
  va_list args;
  int i;

  va_start_ptr(args, ptr);
  i = vsprintf(buf, fmt, args);
  va_end(args);
  con_printr(ring, buf);
  return i;
}

/* Task stuffs */
l_long ldt_core[2] =	{
	0x00c09a0000000000 + MEM_LIMIT,	// Sel. 0x04 CORE_LDT_CODE
	0x00c0920000000000 + MEM_LIMIT	// Sel. 0x0c CORE_LDT_DATA
};

//long stack_core[68];
long stack_task[68];

struct tss_struct tss_core = {
	0,
	0,			// p_stack_core_krn.end_stack
	CORE_DATA,	// ss0
	0,0,		// esp1, ss1
	0,0,		// esp2, ss2
	0,			// cr3
	0,			// ptr 0f devs_task,
	0x1000,		// eflags = IOPL for ring 1 & interrupt disable
	0,0,0,0,	// eax, ebx, ecx, ebx
	0,			// ptr of p_stack_core.end_stack = esp
	0,0,0,		// ebp, esi, edi
	CORE_LDT_DATA, CORE_LDT_CODE, // es, cs
	CORE_LDT_DATA, CORE_LDT_DATA, // ss, ds
	CORE_LDT_DATA, CORE_LDT_DATA, // fs, gs
	LDT_CORE,   // sel of ldt table
	0x8000000   // trace bitmap
};

//struct stack_ptr p_stack_core = { & stack_core[68]};
struct stack_ptr p_stack_task = { & stack_task[68]};

void set_core_stack(void) {

	long * p0 = (long *) & printr_task;

	//tss_core.core_stack = p_stack_core.end_stack;
	tss_core.task_stack = p_stack_task.end_stack;
	tss_core.task = p0;
}

__naked_ void set_core_tss_desc(void) {
__asm{
	add edi,ecx
	mov [edi+0x2], ax
	ror eax, 0x10
	mov [edi+0x4], al
	mov [edi+0x7], ah
	ret
	}
}

void set_core_desc(long * p_gdt) {
__asm{
	mov ecx, [p_gdt]
	lea eax, tss_core
	mov edi, TSS_CORE
	call set_core_tss_desc

	lea eax, ldt_core
	mov edi, LDT_CORE
	call set_core_tss_desc
	}
}

void core_init(long * p_gdt) {
  set_core_desc(p_gdt);
  set_core_stack();
}

/*
 * printr_task: Strange but a task defined in ring 0 can be very useful as a
 * separation layer.
 * The printr_task is necessary as a layer that allows the call rings to keep
 * all their data in the registers and on the stack untouched by the functions
 * that process the data in the core ring.
 * The printr_task is called as a nested task.
 */
__naked_ void printr_task(void){
  __asm{
	task_loop:
	push edx               // push ptr
	push [edx]             // push fmt
	push ecx               // push ring
	call printr
	add esp, 12            // put stack on right position
	mov tss_core.eax, eax
	iretd                  // return feom nested task to _printr
	jmp task_loop
  }
}
/* End task stuffs */

/*
 * This function or sys call is linked in call gate descriptor CG_CORE_PRINTR
 * and is called by long call = lcall CG_CORE_PRINTR,0.
 * Has one input argument which is start of stack at ring x with x arguments.
 * This func forward the first ptr (of fmt arg) to printr_task, making them
 * useful for above printr function.
 *
 * Here printr_task (called as lcall TSS_CORE,0) is necessary as a layer that
 * allows the call ring to keep all the data in the registers and on the stack
 * untouched by the functions that process the data in the core ring.
 */
__naked_ void _printr(void){
  __asm{
	push ecx
	mov ecx, ds                     // save ds from the calling ring
	push ecx
	mov ax, CORE_DATA
	mov ds, ax                      // set ds to core data structure
	and cx,3                        //  get cpl from ring
	mov tss_core.ecx, ecx
	//mov tss_core.ebp, ebp
	mov eax, [esp+16]               // alocate stack from ring x
	mov tss_core.edx, eax
	lcall TSS_CORE,0                // call nested task printr_task
	pop eax
	pop ecx
	mov ds, ax                      // restore ds from caling ring
	retf 4                          // return one arg 4 byte
  }
}
