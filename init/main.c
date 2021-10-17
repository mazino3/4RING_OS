/*
 * 4RING_OS License: MIT
 *
 * main.c
 *
 * Main func prepares all initializations on all rings and entering
 * on main_core_task which is at ring 3 ring (users).
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 *
 */

#include <typedef.h>
#include <sys.h>
#include <gdt.h>
#include <core/page.h>

extern void devs_init(long *);
//extern void libs_init(long *);
extern void users_init(long *);
extern long * p_gdt;

int main(void);
void enter_main_core_task(void);
void sys_init(void);

__naked_ void after_pg_tables() {
__asm{
	push 0
	push 0
	push 0
	lea eax, l1
	push eax
	lea eax, main
	push eax
	jmp setup_paging
	l1:
	jmp l1
	}
}

int main(void) {
	
	sys_init();
	devs_init(p_gdt);
	// libs_init(p_gdt);
	users_init(p_gdt);
	enter_main_core_task();
}

/* Entering the task which selector is in GDT, With long jump asm instruction.
 * Task is in ring 3 (users) with turned on interrupt flag and IO
 * privileged lever at ring 1 to be able to taking interrupts from keyboard
 * and so on, since all devices are at ring 1.
 */
__naked_ void enter_main_core_task(void) {
__asm{
   ljmp TSS_MAIN_TASK, 0
  }
}

void sys_init(void){

  /* Set supervisor flag at PTE for system memory space for SYS_SIZE */
  set_pte_flags(SYS_START, GET_NR_ENTRY( SYS_SIZE ), CLR_ROOT_FL);

}
