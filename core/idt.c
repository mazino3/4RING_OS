/*
 * 4RING_OS License: MIT
 *
 * idt.c
 *
 * Setup interrupt descriptor table at core (ring 0)
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <idt.h>

void comm_ignore_int(void);

l_long idt[256];

idt_des_struc idt_desc = { // Global idt descripor alocated here
	0,				// ptr to the function	
	CORE_CODE,		// a code segment selector
	0,				// unused, set to 0
	0,				// attr_type
	0,				// data/code seg (S=1) or a system segment (S=0)
	0,				// Descriptor Privilege Level (2 bits, 0..3)	
	1,				// (Present) flag
	0				// index place at idt table or IRQ
};

struct idt_opcode_p {
	u_short size;	// 256 entries
	u_int * idt;	// ptr of idt table + core base
} static idt_opcode = {(0x100*8-1), ((u_int *) & idt[0])};


__naked_ void setup_idt(void) {
__asm{
	lea edx, comm_ignore_int
	mov eax,0x00080000
	mov ax,dx
	mov dx,0x8e00
	lea edi,idt
	mov ecx,0x100
	rp:
	mov [edi],eax
	mov [edi+0x4],edx
	add edi, 0x8
	dec ecx
	jnz rp
	lidt idt_opcode
	ret
	}
}

// This is the default interrupt "handler"
__naked_ void comm_ignore_int(void) {
__asm{
	push ds
	push eax
	mov eax,CORE_DATA
	mov ds,eax
	mov eax,0x43 // 67 print '4'
	pop eax
	pop ds
	iretd
	// align 2
	}
}
