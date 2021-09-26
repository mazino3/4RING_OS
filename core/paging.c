/*
 * 4RING_OS License: MIT
 *
 * main.c
 *
 * Here is turning on paging and preparing to jump at main funct.
 * setup_paging function is taken from Linux 0.01
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 *
 */

#include <typedef.h>
#include <sys.h>
#include <core/page.h>

void setup_paging(void);

/* Follows pointers for paging directorium (1) and page tables(2)
 * After the initialization the first 0x3000 bytes
 * of code will be overwritten by these page tables
 */
int * pg_dir0 = (int *) 0x00000000;//Ptr of page dir starts from addr 0x00
int * pg_tab0 = (int *) 0x00001000;//Ptr of page table for first 4Mb(4096 bytes)
int * pg_tab1 = (int *) 0x00002000;//Ptr of page table for another 4Mb = 8Mb
//int * pg_tab2 = (int *) 0x00003000;//Ptr of page table for another 4Mb

__naked void setup_paging(void) {
__asm{
	mov ecx, PTE_SIZE*3
	xor eax, eax
	xor edi, edi
	cld
	rep stosd
	mov ebx, [pg_dir0]
	mov edx, [pg_tab0]
	add edx, 7
	mov dword ptr [ebx], edx
	mov edx, [pg_tab1]
	add edx, 7
	mov dword ptr [ebx+4], edx
	mov edi,	[pg_tab1]
	add edi, 4092
	mov eax, 0x7ff007
	std
	l1:
	stosd
	sub eax, PAGE_SIZE
	jnl l1
	xor eax, eax
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000000
	mov cr0,eax
	ret
	}
}

/* Set supervisor flag at PDT or PTE for system memory space for n entries
 */
void set_pte_flags(u_int addr, u_short nr_entry, u_int flags) {

  /* Get addr of table entry where changing flags */
  u_int * p_adrr = (u_int *)((GET_PTE( addr )+PTE_SIZE)*4);

  for (; nr_entry>0; nr_entry--, p_adrr++){
	  *p_adrr = *p_adrr & flags;
  }

  /* TLB flush method */
  flush_tlb();
}
