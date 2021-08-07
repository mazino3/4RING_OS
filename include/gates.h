/*
 * 4RING_OS License: MIT
 *
 * gates.h
 *
 * Gates definitions
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _GATES_H
#define _GATES_H

#include <typedef.h> 

#define CALL_GATE	0x0C
#define INT_GATE	0x0E
#define TRP_GATE	0x0F
#define TASK_GATE	0x05

typedef struct Call_gate
{
	u_short OffsetLow;
	u_short Selector;
	u_char NumberOfArguments:5;
	u_char Reserved:3;
	u_char Type:5;
	u_char Dpl:2;
	u_char Present:1;
	u_short OffsetHigh;
}call_gate,*p_call_gate;

/* fword argument (6 bytes) for far lcall or ljmp */
struct call_gate_args {
	u_short null0;
	u_short null1;
	u_short selector;
};

extern struct call_gate_args call_args;

#endif /* _GATES_H */
