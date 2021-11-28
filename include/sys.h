/*
 * 4RING_OS License: MIT
 *
 * sys.h
 *
 * Definitions for System memory spaces.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _SYS_H
#define _SYS_H

#include <config.h>
#include <gdt.h>

#define MEM_LIMIT ((MEM_SIZE) / 0x1000) - 1 // 0x7ff

#define CORE_SIZE 36*1024 	// 36Kb
#define DEVS_SIZE 36*1024 	// 36Kb
#define LIBS_SIZE 36*1024 	// 36Kb
#define RESERVE   0
#define USERS_SIZE 36*1024 //MEM_SIZE-GDT_SIZE-CORE_SIZE-DEVS_SIZE-LIBS_SIZE-RESERVE

#define GDT_SIZE	8*GDT_ENTRIES				// 36Kb
#define GDT_START	(MEM_SIZE) - (GDT_SIZE)		// 7F7000

#define CORE_START (GDT_START) - (CORE_SIZE)	// 7EE000
#define DEVS_START (CORE_START) - (DEVS_SIZE)	// 7E5000
#define LIBS_START (DEVS_START) - (LIBS_SIZE)	// 7DC000
#define USERS_START (LIBS_START) - (USERS_SIZE)	// 7D3000
//#define USERS_START 0x100000					// 1Mb

#define SYS_START (LIBS_START)  // not confusing, USERS_START is in SYS domain.
#define SYS_SIZE (MEM_SIZE) - (LIBS_START)

#define DEVS_STACK (CORE_START) - 4
#define LIBS_STACK (DEVS_START) - 4
#define USERS_STACK (LIBS_START) - 4

#define DEVS_LIMIT ((CORE_START) / 0x1000) - 1
#define LIBS_LIMIT ((DEVS_START) / 0x1000) - 1
/* Only for the main task in the user space */
#define USERS_SYS_LIMIT ((LIBS_START) / 0x1000) - 1

#endif /* _SYS_H */

