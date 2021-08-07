/*
 * 4RING_OS License: MIT
 *
 * idt.h
 *
 * Interrupt Descriptor Table definitions
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
  +-----------------+-----------------+---+---+---------+-----+-----------+
  |           OFFSET 31..16           | P |DPL|0 1 1 1 0|0 0 0|(NOT USED) |4
  |-----------------------------------+---+---+---------+-----+-----------|
  |             SELECTOR              |           OFFSET 15..0            |0
  +-----------------+-----------------+-----------------+-----------------+
  */

#ifndef _IDT_H
#define _IDT_H

#include <typedef.h>
#include <gates.h>

//#define INT_GATE 0b1110 // = 0x0E	32-bit interrupt gate

typedef struct Idt_Desc {
   int   *  func;			// ptr to the function
   u_short	selector;		// a code segment selector in GDT or LDT
   u_char	zero;			// unused, set to 0
   u_char	attr_type:4;	// type and attributes
   u_char	segment:1;		// data/code seg (S=1) or a system segment (S=0)
   u_char	dpl:2;			// Descriptor Privilege Level (2 bits, 0..3)
   u_char	present:1;		// (Present) flag
   u_short	tbl_index;		// index place at idt table sample = ( 0x21 for kyboard interrupt)
} idt_des_struc;

extern idt_des_struc idt_desc;

#endif /* _IDT_H */

