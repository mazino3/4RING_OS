#
# 4RING_OS License: MIT
#
# Makefile for 4libc.
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../../defines.mk

INCL_SYS = ../../include
INCL = include/

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I $(INCL_SYS) -I $(INCL) -fdeclspec \
	-fpack-struct -D$(BOOT_WAY)

LDFLAGS =

STATIC =  printf.o
DYNAMIC = _printf.o functs_tbl.o

all: 4libc.a 4libc.o

c.o:
	$(CC) $(CFLAGS) -O -o $*.o $<
	
4libc.a: $(STATIC) 
	$(AR) rcs 4libc.a $(STATIC)
	sync
	
4libc.o: $(DYNAMIC)
	$(LD) -r -o 4libc.o $(DYNAMIC)

clean:
	rm -f *.o -v tmp* *.a

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do clang -E -nostdinc -I $(INCL_SYS) -I $(INCL) -M $$i;done) >> tmp_make
	cp tmp_make Makefile

# Dependencies

_printf.o: _printf.c ../../include/typedef.h ../../include/gdt.h \
  ../../include/stdarg.h ../../include/io.h ../../include/gas_regs.h \
  ../../include/devs/devs_calls.h ../../include/sched.h
functs_tbl.o: functs_tbl.c ../../include/typedef.h
printf.o: printf.c include/libc_calls.h ../../include/typedef.h \
  ../../include/gdt.h ../../include/sched.h ../../include/io.h \
  ../../include/gas_regs.h ../../include/libs/lib_defs.h
