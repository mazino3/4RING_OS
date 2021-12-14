/*
 * 4RING_OS License: MIT
 *
 * keyboard.c
 *
 * The code is mostly inspired by Linux version 0.01
 *
 * Keyboard functions are covered here. The interrupt is quickly caught
 * and the value of the key is recorded in the structure so that the program
 * flow is redirected to the task (devs_irq_task) which by default has the
 * interrupt flag turned off. Task is further prepared to process the value
 * after the first entering from core_schedule (ring 0) to the devs_schedule
 * at (ring 1).
 *
 * The functionality of keyboard is't yet fully implemented!!!
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <idt.h>
#include <io.h>
#include <tty.h>
#include <interupt.h>
#include <task.h>

extern struct tss_struct tss_devs_irq;
extern void save_irq(u_int val, void_func func);

u_char e0, leds, mode;
int is_none = 0;

const u_char key_map[] = {
	0,27,
	'1','2','3','4','5','6','7','8','9','0','-','=',            	
	127,9,
	'q','w','e','r','t','y','u','i','o','p','[',']',	 
	10,0,
	'a','s','d','f','g','h','j','k','l',';','\'','`',
	0,
	'\\','z','x','c','v','b','n','m',',','.','/',
	0,'*',0,32,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	'-',0,0,0,'+',
	0,0,0,0,0,0,0,'<',
	0,0,0,0,0,0,0,0,0,0
};

const u_char shift_map[] = {
	0,27,
	'!','@','#','$','%','^','&','*','(',')','_','+',
	127,9,
	'Q','W','E','R','T','Y','U','I','O','P','{','}',
	10,0,
	'A','S','D','F','G','H','J','K','L',':','"','~',	
	0,
	'|','Z','X','C','V','B','N','M','<','>','?','_',
	0,'*',0,32,	 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	'-',0,0,0,'+',
	0,0,0,0,0,0,0,'>',
	0,0,0,0,0,0,0,0,0,0
};	

__naked_ void put_queue(void) {
__asm{
	push ecx
	mov edx, tty_tbl_list[0]				//read_q
	mov ecx, [edx + _head]
	mov byte ptr[edx + ecx*1 + _buf], al
	inc ecx									//[keyb_queue.head]
	and ecx, TTY_BUF_SIZE-1
	cmp ecx, [edx + _tail]					//tail buf full, discard everything
	je l1
	mov [edx + _head], ecx
	l1:
	pop ecx
	ret
	}
}	

__naked_ void do_key(void) {
__asm{
	//lea ebx,[alt_map]
	//test byte[mode], 0x20					// alt-gr
	//jne l1
	lea ebx,[shift_map]
	test byte ptr[mode], 0x03
	jne l1 
	lea ebx,[key_map]
l1:
	mov al,[ebx + eax*1]
	or al,al
	je none
	test byte ptr[mode],0x4c				// ctrl or caps
	je l2
	cmp al,'a'
	jb l2
	cmp al,'z'
	ja l2
	sub al,32
l2:
	test byte ptr[mode],0x0c				// ctrl
	je l3
	cmp al,64
	jb l3
	cmp al,64+32
	jae l3
	sub al,64
l3:
	test byte ptr[mode],0x10				// left alt
	je l4
	or al,0x80
l4:
	and eax,0xff
	xor ebx,ebx
//
	call put_queue 
none:
	ret 
	}
}

__naked_ void ctrl(void) {
__asm{
	nop
	ret
  }
}

__naked_ void none(void) {
__asm{
	mov [is_none], 1
	ret
  }
}

__naked_ void lshift(void) {
__asm{
	or byte ptr[mode],0x01
	ret
  }
}

__naked_ void rshift(void) {
__asm{
	or byte ptr[mode],0x02
	ret
  }
}

__naked_ void alt(void) {
__asm{
	nop
	ret
  }
}

__naked_ void caps(void) {
__asm{
	nop
	ret
  }
}

__naked_ void func(void) {
__asm{
	nop
	ret
  }
}

__naked_ void curs(void) {
__asm{
	nop
	ret
  }
}

__naked_ void unctrl(void) {
__asm{
	nop
	ret
  }
}

__naked_ void unlshift(void) {
__asm{
	and byte ptr[mode],0xfe
	ret
  }
}

__naked_ void unrshift(void) {
__asm{
	and byte ptr[mode],0xfd
	ret
  }
}

__naked_ void unalt(void) {
__asm{
	nop
	ret
  }
}

__naked_ void uncaps(void) {
__asm{
	nop
	ret
  }
}

__naked_ void num(void) {
__asm{
	nop
	ret
  }
}

__naked_ void scroll(void) {
__asm{
	nop
	ret
  }
}

/* Halt processor */
__naked_ void halt(void) {
__asm{
	mov al, 0xfe
	out 0x64, al
  }
}

void_func key_tbl[] = {
	none, do_key, do_key, do_key, do_key, do_key, do_key, do_key,	// 00-07 s0 esc 1 2 3 4 5 6
	do_key, do_key, do_key, do_key, do_key, do_key, do_key, do_key,	// 08-0F 7 8 9 0 - = bs tab
	do_key, do_key, do_key, do_key, do_key, do_key, do_key, do_key,	// 10-17 q w e r t y u i
	do_key, do_key, do_key, do_key, do_key, ctrl, do_key, do_key,	// 18-1F o p [ ] enter ctrl a s
	do_key, do_key, do_key, do_key, do_key, do_key, do_key, do_key,	// 20-27 d f g h j k l ;
	do_key, do_key, lshift, do_key, do_key, do_key, do_key, do_key,	// 28-2F ' ` lshift \ z x c v
	do_key, do_key, do_key, do_key, do_key, do_key, rshift, do_key,	// 30-37 b n m , . / rshift *
	alt, do_key, caps, func, func, func, func, func,	// 38-3F  alt sp caps f1 f2 f3 f4 f5
	func, func, func, func, func, num, scroll, curs,	// 40-47  f6 f7 f8 f9 f10 num src home
	curs, curs, do_key, curs, curs, curs, do_key, curs,	// 48-4F  up pgup - left n5 right + end
	curs, curs, curs, curs, none, none, do_key, func,	// 50-57  dn pgdn ins del sysreq ? < f11
	func, none, none, none, none, none, none, none,	// 58-5F  f12 ? ? win_key ? ? ? ?
	none, none, none, none, none, none, none, none,	// 60-67  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,	// 68-6F  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,	// 70-77  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,	// 78-7F  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,	// 80-87  ? br br br br br br br
	none, none, none, none, none, none, none, none,	// 88-8F  br br br br br br br br
	none, none, none, none, none, none, none, none,	// 90-97  br br br br br br br br
	none, none, none, none, none, unctrl, none, none,	// 98-9F  br br br br br unctrl br br
	none, none, none, none, none, none, none, none,		// A0-A7  br br br br br br br br
	none, none, unlshift, none, none, none, none, none,	// A8-AF  br br unlshift br br br br br
	none, none, none, none, none, none, unrshift, none,	// B0-B7  br br br br br br unrshift br
	unalt, none, uncaps, none, none, none, none, none,	// B8-BF  unalt br uncaps br br br br br
	none, none, none, none, none, none, none, none,		// C0-C7  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,		// C8-CF  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,		// D0-D7  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,		// D8-DF  br ? ? uwin_key ? ? ? ?
	none, none, none, none, none, none, none, none,		// E0-EF  e0 e1 ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,		// E8-EF  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,		// F0-F7  ? ? ? ? ? ? ? ?
	none, none, none, none, none, none, none, none,		// F8-FF  ? ? ? ? ? ? ? ?
};

 /* This function is not called from the keyboard interrupt func, but from
  * the devs_sched_task function, and even more correctly from the Task.
  */
void keyboard(u_int value) {
__asm{
	mov eax, value					// get value from table
	cmp al, 0xe0
	je set_e0
	cmp al, 0xe1
	je set_e1
	call [key_tbl + eax * 4]
	mov byte ptr [e0],0

	e0_e1:
	mov ebx,[is_none]
	cmp ebx,1
	je exit

	push 0x0
	call tty_interrupt
	add esp,0x04
	jmp short exit

	set_e0:
	mov byte ptr [e0],1
	jmp short e0_e1
	set_e1:
	mov byte ptr [e0],2
	jmp short e0_e1

	exit:
	mov [is_none], 0
  }
}

/* get key interrupt */
__naked_ void get_key_int(void) {
__asm{
	xor al,al
	in al,0x60
	xor ebx,ebx
	mov ebx,eax

	lea eax, keyboard	// arg func dword ptr gs:
	push eax			// arg func
	push ebx			// arg val
	call save_irq		// for now every push & up key is saved!
	add esp,0x08

	in al,0x61
	jmp n1
	n1:
	or al,0x80
	jmp n2
	n2:
	out 0x61,al
	jmp n3
	n3:
	and al,0x7f
	out 0x61,al
	mov al,0x20
	out 0x20,al
	ret
  }
}

__naked_ void Keyb_int(void) {
__asm{
	// nop // For debugging reasons on Bochs
	mov dword ptr gs:[tss_devs_irq.ebx], KEY_INT // Put irq request at task.ebx
	lcall TSS_DEVS_IRQ,0 // Call task to be nested where IF is clear
	iretd                // Return from the nested task
  }
}
