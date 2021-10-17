/*
 * 4RING_OS License: MIT
 *
 * timer.c
 *
 * Setup up timer 8253 chip
 * 8253/8254 PIT - Programmable Interval Timer
 *
 * Functions are taken from Linux version 0.01 and modified.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <idt.h>
#include <gates.h>

extern void sched(void);
extern void set_timer_interrupt(void);

void timer_interrupt(void);

__naked_ void setup_timer(void) {
__asm{
	mov al,	0x36
	mov edx, 0x43
	out dx,	al
	mov eax, 11930	// For imer frequency 100 Hz = 11930
	mov edx, 0x40
	out dx,	al
	mov al,	ah
	out dx,	al
	ret
	}
}

__naked_ void umask_tim_int(void) {
__asm{
	mov edx, 0x21
	in al, dx
	and al, 0xfe
	out dx, al
	ret
	}
}

__naked_ void timer_interrupt(void) {
__asm{
	nop				// for debbug reasons at bochs
	push ds
	push edx
	push ecx
	push ebx
	push eax
	mov eax,CORE_DATA
	mov ds,eax
	mov al,0x20
	out 0x20,al
	call sched
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop ds
	iretd
  }
}

void activate_timer(void) {
  setup_timer();
  set_timer_interrupt();
  umask_tim_int();
}

