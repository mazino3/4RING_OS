/*
 * 4RING_OS License: MIT
 *
 * 4libc/_printf.c
 *
 * Part of 4libc dynamic 4libc.o for 4RING_OS
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 *
 */

#include <typedef.h>
#include <gdt.h>
#include <stdarg.h>
#include <io.h>
#include <devs/devs_calls.h>

extern int vsprintf (char * buf, const char * fmt, va_list args );

static char buf[1024];

int _printf(const char *fmt, void *ptr)
{
  va_list args;
  int i;

  va_start_ptr(args, ptr);
  i = vsprintf(buf, fmt, args);
  va_end(args);
  tty_write(0,buf,i); // tty_write here is static inline call gate.
  return i;
}

/*
 * This function is called from libs_irq_task.
 * In the future it should be called from libs_sched_task.
 */
__naked_ void _printf_(void){
  __asm{
	push edx
	push [edx]
  	call _printf
	add esp, 8
  	ret
    }
}
