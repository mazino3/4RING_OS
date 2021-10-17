/*
 * 4RING_OS License: MIT
 *
 * init32.c
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <task.h>
#include <gdt.h>
#include <tty.h>

#define PAGE_SIZE 4096

extern long long gdt[];

extern void setup_idt(void);
extern void setup_gdt(void);
extern void call_write();
extern void setup_timer();
extern void activate_timer(void);
extern void set_interrupts(void);
extern void umask_tim_int();
extern void sc_sched_req(void);

void after_pg_tables();
void set_tss_desc(void);
static inline void setup_descriptors (void);
void set_call_gate(void);

/* stack start addres */
long kernel_stack [PAGE_SIZE>>4];

struct stack_descript {
	long * ptr_end_stack;
	short  base;
} stack_start = { & kernel_stack[PAGE_SIZE>>4], CORE_DATA };
	
__naked_ void  init_32(void) {
  __asm{
	mov eax, CORE_DATA
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	lss esp, stack_start
	jmp setup_descriptors
	}
}

long * p_gdt = (long *) &gdt[0];

__naked_ void  setup_descriptors(void) {
  __asm{
	//mov ebx, CORE_BASE
	mov ecx, [p_gdt]

	lea eax, sc_sched_req
	mov edi, CG_CORE_TX_IRQ
	call set_call_gate

	call setup_idt
	call setup_gdt

	mov eax, CORE_DATA
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	lss esp, stack_start

	call activate_timer
	call set_interrupts

	xor eax,eax
	l1:
	inc eax				// check that A20 really IS enabled
	mov [0],eax
	cmp [0x100000],eax
	jz l1
	mov eax,cr0			// check math chip
	and eax,0x80000011	// Save PG,ET,PE
	test eax,0x10
	jnz l2
	or eax,4
	l2:
	mov cr0,eax
	// Make sign if is ok
	mov eax,0x234		// Green number 4
	mov [0xb8000], ax
	jmp after_pg_tables
	}
}

__naked_ void set_tss_desc(void)	{
  __asm{
	add edi,ecx
	mov [edi+0x2], ax
	ror eax, 0x10
	mov [edi+0x4], al
	mov [edi+0x7], ah
	ret
	}
}

__naked_ void set_call_gate(void) {
  __asm{
	add edi,ecx
	mov [edi], ax
	ror eax, 0x10
	mov [edi+0x6], ax
	ret
	}
}




