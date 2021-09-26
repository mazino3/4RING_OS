/*
 * 4RING_OS License: MIT
 *
 * core/page.h
 *
 * Definitions for paging
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _PAGE_H
#define _PAGE_H

#define PAGE_SIZE  4096  // Size of bytes
#define PDE_SIZE   1024  // Nr entries of page dirextoruim
#define PTE_SIZE   1024  // Nr entries of page table

#define GET_PDE(addr) ((addr)/PAGE_SIZE*PTE_SIZE)
#define GET_PTE(addr) ((addr)/PAGE_SIZE)
#define GET_NR_ENTRY(mem_size) ((mem_size)/PAGE_SIZE)

#define CLR_ROOT_FL 0xfffffffd

extern void setup_paging(void);
extern void set_pte_flags(u_int addr, u_short nr_entry, u_int flags);

#define flush_tlb() ({	  \
	__asm{ mov eax, cr3 } \
	__asm{ mov cr3, eax } \
})

#endif /* _PAGE_H */
