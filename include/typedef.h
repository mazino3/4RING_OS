/*
 * 4RING_OS License: MIT
 *
 * typedef.h
 *
 * Basic typedefs
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _TYPE_DEF
#define _TYPE_DEF


typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned int	u_int;
typedef unsigned long  	u_long;
typedef long long  		l_long;

typedef void (*void_func)(void);
typedef void (*void_f_int)(u_int);

#define NULL ((void *)0)
#define __naked    __declspec(naked)
#define __inline_  __attribute__((always_inline))
#define __noret_   __attribute__((__noreturn__))
#define INT_SIZE 4

#endif /* _TYPE_DEF */
