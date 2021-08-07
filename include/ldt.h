/*
 * 4RING_OS License: MIT
 *
 * ldt.h
 *
 * Defines for local descriptor tables
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _LDT_H
#define _LDT_H

/* SELECTORS FOR LOCAL DESC TABLES */

// At ring 1
#define DEVS_IRQ_CODE	0x05
#define DEVS_IRQ_DATA	0x0d
#define DEVS_SCHED_CODE	0x05
#define DEVS_SCHED_DATA	0x0d

// At ring 2
/* ... */

// At ring 3
#define TASK_CODE  0x07	// TASK_USER_CODE sel.
#define TASK_DATA  0x0f	// TASK_USER_DATA sel.

#endif /* _LDT_H */

