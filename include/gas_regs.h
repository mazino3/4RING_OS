/*
 * 4RING_OS License: MIT
 *
 * gas_regs.h
 *
 * Basic MACROS for using registers
 * 
 * Defines basic registers so that they can be assigned
 * by values to avoid direct use of 'heavy' GAS syntax.
 *
 * NOTE! Values are automatically assigned to the registry by type of
 * (value): for example, if the type of value is (char) then the
 * expresion _eax(c); = mov al,c but _eax((long) c) = mov eax,c.
 *
 * Here the mixed Intel & Gas syntax, is posible at same time by using
 * the LLVM's Clang compiler.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _GAS_REGS
#define _GAS_REGS

/* 
 * Macros for seting registers with value. 
 * The minimum inevitable GAS syntax is used here
 */
#define _esi(value) __asm__ (""::"S" (value))
#define _edi(value) __asm__ (""::"D" (value))
#define _ecx(value) __asm__ (""::"c" (value))
#define _eax(value) __asm__ (""::"a" (value))
#define _ebx(value) __asm__ (""::"b" (value))
#define _edx(value) __asm__ (""::"d" (value))

/* Intel syntax */
#define _cli() __asm__ {cli}
#define _sti() __asm__ {sti}

#endif /* _GAS_REGS */

