/*
 * 4RING_OS License: MIT
 *
 * libs/lib_sys.h
 *
 * System calls thru the lib_sys.a static library.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef __LIB_SYS
#define __LIB_SYS

#include <core/sys_calls.h>

extern int printr(const char *fmt, ...);

#endif /* __LIB_SYS */
