/*
 * 4RING_OS License: MIT
 *
 * interupts.c
 *
 * Setup interrupts at core (ring 0) and devs (ring 1 just for beginning)
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */
/* 
  
	  15  14  13  12  11  10  9   8  
	+---+---+---+---+---+---+---+---+
	| P |  DPL  | S |    GateType   | 
	+---+---+---+---+---+---+---+---+
	
	80386 INTERRUPT GATE
   31                23                15                7                0
  +-----------------+-----------------+---+---+-+-------+-----+-----------+
  |           OFFSET 31..16           | P |DPL|0|1 1 1 0|0 0 0|(NOT USED) |4
  |-----------------------------------+---+---+---------+-----+-----------|
  |             SELECTOR              |           OFFSET 15..0            |0
  +-----------------+-----------------+-----------------+-----------------+
*/

#include <typedef.h>
#include <idt.h>
#include <gdt.h>
#include <io.h>

#include <devs/hard_int.h>

extern int idt;
/* The capital letter of the function means that it is from another ring */
extern void Keyb_int(void);
extern void timer_interrupt(void);

__naked void set_interrupt( void ) { 
__asm{
	mov ebx, idt_desc.func
	xor eax, eax
	xor ecx, ecx
	mov ax, idt_desc.selector
	ror eax, 0x10
	mov ax, bx
	mov cx, idt_desc.tbl_index
	mov dx,0
	mov dh, idt_desc.attr_type
	mov bx, dx
	lea esi, [ecx*8+idt]
	mov [esi], eax
	mov [esi+4], ebx
	ret
	}
}

void set_timer_interrupt(void) {

	idt_desc.attr_type = INT_GATE;
	idt_desc.tbl_index = TIMER_INT;
	idt_desc.func = ((int *) & timer_interrupt);	// addr of function
	idt_desc.selector = CORE_CODE;					// selector at Ring 0
	idt_desc.dpl = DPL_RING0;
	/* Followings members are initiated by default for system segment P=1 S=0 */
	//idt_desc.present = 1;
	//idt_desc.segment = 0;
	set_interrupt(); 
}

/* For further this func must be at ring 1 (devs) */
void set_keyb_interrupt(void) {

	register u_char a;

	idt_desc.attr_type = INT_GATE;
	idt_desc.tbl_index = KEYB_INT;
	idt_desc.func = ((int *) & Keyb_int);			// addr of function
	idt_desc.selector = DEVS_CODE;					// selector at Ring 1
	idt_desc.dpl = DPL_RING1;
	
	set_interrupt();
	
	outb_p( inb_p( 0x21 ) & 0xfd, 0x21);
	a = inb_p( 0x61 );
	outb_p( a | 0x80, 0x61);
	outb( a, 0x61);
}

void set_interrupts(void) {
  set_keyb_interrupt();
}
