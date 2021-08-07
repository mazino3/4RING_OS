/*
 * 4RING_OS License: MIT
 *
 * io.h
 *
 * (C) Copyright 2019 Isa <isa@isoux.org>
 */

#ifndef _IO_H
#define _IO_H

#include <gas_regs.h>

/* The following terms are inevitable to be automatic
  * assigned values to the register and are given
  * decides whether there is a return or a forward or not.
  */
#define _return(value) __asm__ volatile ("":"=a" (value):)
#define _forward(value) __asm__ volatile (""::"a" (value))

/* jmp next line (2 times) */
#define _jmp_x2 __asm__ volatile ("jmp 1f\n" "1: jmp 2f\n" "2: ") 



/* Introducing the new policy of using
 * mixed assembler syntax, with goal
 * to be more understanding & readable, I hope :-)
 */
 
#define inb(port) ({		\
unsigned  char _v;			\
	_edx((short)port);		\
	__asm__ {in al, dx}		\
	_return(_v);			\
	_v;						\
})

#define inb_p(port) ({		\
unsigned char _v;			\
	_edx((short)port);		\
	__asm__ {in al, dx}		\
	_jmp_x2;				\
	_return(_v);			\
	_v;						\
})

#define outb(value,port) ({	\
   _eax(value);				\
   _edx((short)port);		\
   __asm__ {out dx, al}		\
  _forward(value);			\
})

#define outb_p(value,port) ({	\
   _eax(value);					\
   _edx((short)port);			\
   __asm__ {out dx, al}			\
   _jmp_x2;						\
	_forward(value);			\
})

#endif /* _IO_H */
