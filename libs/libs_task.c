/*
 * 4RING_OS
 *
 * libs_task.c
 *
 * For now tasks are statically initialized at memory
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <gdt.h>
#include <ldt.h>
#include <task.h>
#include <sys.h>

extern void libs_irq_task(void);
//extern void libs_sched_task(void);


void libs_init(long * p_gdt);

l_long ldt_irq[2] =	{
	0x00c0da0000000000 + LIBS_LIMIT,	// Sel. 0x06 LIBS_IRQ_CODE
	0x00c0d20000000000 + LIBS_LIMIT		// Sel. 0x0e LIBS_IRQ_DATA
};
/*
l_long ldt_sched[2] = {
	0x00c0da0000000000 + LIBS_LIMIT,	// Sel. 0x06 LIBS_SCHED_CODE
	0x00c0d20000000000 + LIBS_LIMIT		// Sel. 0x0e LIBS_SCHED_DATA
};
*/
long stack_libs_irq[68];
long stack_libs_irq_core[68];
long stack_libs_irq_devs[68];

//long stack_libs_sched[68];
//long stack_libs_sched_core[68];
//long stack_libs_sched_devs[68];

struct tss_struct tss_libs_irq = {
	0,
	0,			// p_stack_core_krn.end_stack
	CORE_DATA,	// ss0
	0,DEVS_DATA,// esp1, ss1
	0,0,		// esp2, ss2
	0,			// cr3
	0,			// ptr 0f libs_task,
	0x1000,		// eflags = IOPL for ring 1 & interrupt disable
	0,0,0,0,	// eax, ebx, ecx, ebx
	0,			// ptr of p_stack_core.end_stack = esp
	0,0,0,		// ebp, esi, edi
	LIBS_IRQ_DATA, LIBS_IRQ_CODE,		// es, cs
	LIBS_IRQ_DATA, LIBS_IRQ_DATA,		// ss, ds
	LIBS_IRQ_DATA, LIBS_IRQ_DATA,		// fs, gs
	LDT_LIBS_IRQ,						// sel of ldt table
	0x8000000							// trace bitmap
};
/*
struct tss_struct tss_libs_sched = {
	0,
	0,			// p_stack_core_krn.end_stack
	CORE_DATA,	// ss0
	0,0,		// esp1, ss1
	0,0,		// esp2, ss2
	0,			// cr3
	0,			// ptr 0f libs_task,
	0x1200,		// eflags = IOPL for ring 1 & interrupt enable
	0,0,0,0,	// eax, ebx, ecx, ebx
	0,			// ptr of p_stack_core.end_stack = esp
	0,0,0,		// ebp, esi, edi
	LIBS_SCHED_DATA, LIBS_SCHED_CODE,	// es, cs
	LIBS_SCHED_DATA, LIBS_SCHED_DATA,	// ss, ds
	LIBS_SCHED_DATA, LIBS_SCHED_DATA,	// fs, gs
	LDT_LIBS_SCHED,						// sel of ldt table
	0x8000000							// trace bitmap
};
*/
struct stack_ptr p_stack_irq = { & stack_libs_irq[64]};
struct stack_ptr p_stack_irq_krn = { & stack_libs_irq_core[64]};
struct stack_ptr p_stack_irq_devs = { & stack_libs_irq_devs[64]};

//struct stack_ptr p_stack_sched = { & stack_tss_sched[64]};
//struct stack_ptr p_stack_sched_krn = { & stack_tss_sched_core[64]};

void set_libs_tasks(void) {

	long * p0 = (long *) & libs_irq_task;

	tss_libs_irq.core_stack = p_stack_irq_krn.end_stack;
	tss_libs_irq.task_stack = p_stack_irq.end_stack;
	tss_libs_irq.ring1_stack = p_stack_irq_devs.end_stack;
	tss_libs_irq.task = p0;

	//p0 = (long *) & libs_sched_task;

	//tss_libs_sched.core_stack = p_stack_sched_krn.end_stack;
	//tss_libs_sched.task_stack = p_stack_sched.end_stack;
	//tss_libs_sched.task = p0;

}

__naked_ void set_libs_tss_desc(void) {
__asm{
	add edi,ecx
	mov [edi+0x2], ax
	ror eax, 0x10
	mov [edi+0x4], al
	mov [edi+0x7], ah
	ret
	}
}

void set_libs_desc(long * p_gdt) {
__asm{
	mov ecx, [p_gdt]

	lea eax, tss_libs_irq
	mov edi, TSS_LIBS_IRQ
	call set_libs_tss_desc

	//lea eax, tss_libs_sched
	//mov edi, TSS_LIBS_SCHED
	//call set_libs_tss_desc

	lea eax, ldt_irq
	mov edi, LDT_LIBS_IRQ
	call set_libs_tss_desc

	//lea eax, ldt_sched
	//mov edi, LDT_LIBS_SCHED
	//call set_libs_tss_desc
	}
}

void libs_init(long * p_gdt) {
  set_libs_desc(p_gdt);
  set_libs_tasks();
}
