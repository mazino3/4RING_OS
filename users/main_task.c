/*
 * 4RING_OS License: MIT
 *
 * main_task.c
 *
 * For now main task are statically initialized at memory
 * space of users (ring 3).
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <ldt.h>
#include <task.h>
#include <sys.h>
#include <libs/lib_sys.h>
#include <rings.h>

#define HELLO_APP 0x700000 // mem address of hello aplication.

void main_task(void);

int cur_task = 0;

l_long ldt_main[3] =	{
	0x00c0fa0000000000 + USERS_SYS_LIMIT, // Sel. 0x07 TASK_CODE
	0x00c0f20000000000 + USERS_SYS_LIMIT, // Sel. 0x0f TASK_DATA
	//0x0000ec02002A0000 // Call gate decs. Sell 0x17
};

long stack_tss_main[68];		// 4 * long size more for align
long stack_tss_main_krn[68];	// Stack for ring 0
long stack_tss_main_devs[68];	// Stack for ring 1
long stack_tss_main_libs[68];	// Stack for ring 2

struct stack_ptr p_stack_main = { & stack_tss_main[64]};
struct stack_ptr p_stack_main_krn = { & stack_tss_main_krn[64]};
struct stack_ptr p_stack_main_devs = { & stack_tss_main_devs[64]};
struct stack_ptr p_stack_main_libs = { & stack_tss_main_libs[64]};

struct tss_struct tss_main = {
	0,				// back link
	0,				// p_stack_core_krn.end_stack
	CORE_DATA,		// ss0
	0,DEVS_DATA,	// esp1, ss1
	0,LIBS_DATA,	// esp2, ss2
	0,				// cr3
	0,				// ptr of main_task,
	0x1200,			// eflags interupt enable IO = 1
	0,0,0,0,		// eax, ebx, ecx, edx
	0,				// ptr of p_stack_core.end_stack = esp
	0,0,0,			// ebp, esi, edi
	TASK_DATA, TASK_CODE, TASK_DATA, TASK_DATA, TASK_DATA, USRS_ACCES_DATA,
					// es, cs, ss, ds, fs, gs
	LDT_MAIN_TASK,	// selector at GDT of ldt table
	0x8000000		// trace bitmap
};

void set_task_stacks(void) {

	long * p0 = (long *) & main_task;

	tss_main.task_stack = p_stack_main.end_stack;
	tss_main.core_stack = p_stack_main_krn.end_stack;
	tss_main.ring1_stack = p_stack_main_devs.end_stack;
	tss_main.ring2_stack = p_stack_main_libs.end_stack;
	tss_main.task = p0;
}


/* From here we will enter the first registered user process. */
void main_task(void) {
  int uid = 1000;
  /* Alert from first time initialization */
  printr("Hello from users (ring 3)! uid = %d\r\n",uid);
  __asm{
	mov eax, HELLO_APP  // call hello application for show ...
	call eax
	loop:
	jmp loop
  }

}

__naked_ void set_usrs_tss_desc(void) {
__asm{
	add edi,ecx
	mov [edi+0x2], ax
	ror eax, 0x10
	mov [edi+0x4], al
	mov [edi+0x7], ah
	ret
	}
}

__naked_ void set_libs_call_gate(void) {
  __asm{
	add edi,ecx
	mov [edi], ax
	ror eax, 0x10
	mov [edi+0x6], ax
	ret
	}
}

void set_usrs_desc(long * p_gdt) {
__asm{
	mov ecx, [p_gdt]

	lea eax, tss_main
	mov edi, TSS_MAIN_TASK
	call set_usrs_tss_desc

	lea eax, ldt_main
	mov edi, LDT_MAIN_TASK
	call set_usrs_tss_desc
	}
}


void users_init(long * p_gdt) {
  set_usrs_desc(p_gdt);
  //set_usrs_tasks();
  set_task_stacks();
}

