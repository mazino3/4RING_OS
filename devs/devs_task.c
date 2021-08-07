/*
 * 4RING_OS
 *
 * devs_task.c
 *
 * For now tasks are statically initialized at memory
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <gdt.h>
#include <ldt.h>
#include <task.h>
#include <sys.h>

extern void devs_irq_task(void);
extern void devs_sched_task(void);
extern void con_init(void);

void Devs_init(long * p_gdt);

l_long ldt_irq[2] =	{
	0x00c0ba0000000000 + DEVS_LIMIT,	// Sel. 0x05 DEVS_IRQ_CODE
	0x00c0b20000000000 + DEVS_LIMIT		// Sel. 0x0d DEVS_IRQ_DATA
};

l_long ldt_sched[2] = {
	0x00c0ba0000000000 + DEVS_LIMIT,	// Sel. 0x05 DEVS_SCHED_CODE
	0x00c0b20000000000 + DEVS_LIMIT		// Sel. 0x0d DEVS_SCHED_DATA
};

long stack_tss_irq[66];					// 2 * long size more for align
long stack_tss_irq_core[64];

long stack_tss_sched[66];				// 2 * long size more for align
long stack_tss_sched_core[64];

struct tss_struct tss_devs_irq = {
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
	DEVS_IRQ_DATA, DEVS_IRQ_CODE,		// es, cs
	DEVS_IRQ_DATA, DEVS_IRQ_DATA,		// ss, ds
	DEVS_IRQ_DATA, DEVS_ACCES_DATA,		// fs, gs
	LDT_DEVS_IRQ,						// sel of ldt table
	0x8000000							// trace bitmap
};

struct tss_struct tss_devs_sched = {
	0,
	0,			// p_stack_core_krn.end_stack
	CORE_DATA,	// ss0
	0,0,		// esp1, ss1
	0,0,		// esp2, ss2
	0,			// cr3
	0,			// ptr 0f devs_task,
	0x1200,		// eflags = IOPL for ring 1 & interrupt enable
	0,0,0,0,	// eax, ebx, ecx, ebx
	0,			// ptr of p_stack_core.end_stack = esp
	0,0,0,		// ebp, esi, edi
	DEVS_SCHED_DATA, DEVS_SCHED_CODE,	// es, cs
	DEVS_SCHED_DATA, DEVS_SCHED_DATA,	// ss, ds
	DEVS_SCHED_DATA, DEVS_ACCES_DATA,	// fs, gs
	LDT_DEVS_SCHED,						// sel of ldt table
	0x8000000							// trace bitmap
};

struct stack_ptr p_stack_irq = { & stack_tss_irq[64]};
struct stack_ptr p_stack_irq_krn = { & stack_tss_irq_core[64]};

struct stack_ptr p_stack_sched = { & stack_tss_sched[64]};
struct stack_ptr p_stack_sched_krn = { & stack_tss_sched_core[64]};

void set_devs_tasks(void) {

	long * p0 = (long *) & devs_irq_task;

	tss_devs_irq.core_stack = p_stack_irq_krn.end_stack;
	tss_devs_irq.task_stack = p_stack_irq.end_stack;
	tss_devs_irq.task = p0;

	p0 = (long *) & devs_sched_task;

	tss_devs_sched.core_stack = p_stack_sched_krn.end_stack;
	tss_devs_sched.task_stack = p_stack_sched.end_stack;
	tss_devs_sched.task = p0;

}

__naked void set_devs_tss_desc(void) {
__asm{
	add edi,ecx
	mov [edi+0x2], ax
	ror eax, 0x10
	mov [edi+0x4], al
	mov [edi+0x7], ah
	ret
	}
}

void set_devs_desc(long * p_gdt) {
__asm{
	mov ecx, [p_gdt]

	lea eax, tss_devs_irq
	mov edi, TSS_DEVS_IRQ
	call set_devs_tss_desc

	lea eax, tss_devs_sched
	mov edi, TSS_DEVS_SCHED
	call set_devs_tss_desc

	lea eax, ldt_irq
	mov edi, LDT_DEVS_IRQ
	call set_devs_tss_desc

	lea eax, ldt_sched
	mov edi, LDT_DEVS_SCHED
	call set_devs_tss_desc
	}
}

void Devs_init(long * p_gdt) {
  set_devs_desc(p_gdt);
  set_devs_tasks();
  con_init();
  //setup_devs_ldt_desc();
}


