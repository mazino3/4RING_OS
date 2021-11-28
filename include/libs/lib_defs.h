/*
 * 4RING_OS License: MIT
 *
 * libs/lib_defs.h
 *
 * Defines for major names for all libs which can be loaded at libs ring (2).
 * Minor defines can be names for functions on lib.
 * Sample:
 * Function printf from libc should be defined like (major, minor)= (1, 1).
 * Where:
 * Major is most significant word (MSW) at 32 bit common integer
 * Minor is least significant word (LSW) at 32 bit common integer.
 *
 * I need this or a similar classification of definitions to use them with
 * call gates from user-space.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _LIB_DEFS
#define _LIB_DEFS

#define MAJOR(a) ((a)<<16)
#define MINOR(a) ((a)&0xFFFF)

/* Major defines for Libs */
#define LIB_SYS  0  // Major name for all Internal libs  for system.
#define LIBC     1  // Standard libc for 4RING_OS.
#define LIBX     2  // Sample for some lib.

#endif /* _LIB_DEFS*/
