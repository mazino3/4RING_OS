/*
 * 4RING_OS License: MIT
 *
 * task.h
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _TASK_H
#define _TASK_H

/* Structure for setting stack pointers into tss structure */
struct stack_ptr {
	long * end_stack;
};

//#define NR_TASKS	0x04			// Max number of tasks

struct tss_struct {
	long	back_link;				// 16 high bits zero 
	long	* core_stack;			// ptr of core_stack for task
	long	core_st_seg;			// core (ring 0) stack segment
	long	* ring1_stack;			// ptr of rin1_stack
	long	ring1_st_seg;			// ring1 stack segment 
	long	* ring2_stack;			// ptr of ring2_stack
	long	ring2_st_seg;			// ring2 stack segment
	long	cr3;
	long	* task;					// ptr of task procedure - function, eip;
	long	eflags;
	long	eax, ecx, edx, ebx;
	long	* task_stack;			// ptr of task-stack 
	long	ebp, esi, edi;
	long	es, cs, ss, ds;			// 16 high bits zero
	long	fs, gs;					// 16 high bits zero
	long	ldt;					// local desc table,  selector
	long	trace_bitmap;			// bits: trace 0, bitmap 16-31
};

typedef struct Tss_Desc {
   u_short  lim_0_15;
   u_short	base_0_15;
   u_char	base_23_16;
   u_char	type:4;
   u_char	S:1;
   u_char	dpl:2;
   u_char	present:1;
   u_char	lim_16_19:4;
   u_char	avl:1;
   u_char	null:1;
   u_char	X:1;
   u_char	G:1;
   u_char base_24_31;
} tss_des_struct;

#endif /* _TASK_H */
