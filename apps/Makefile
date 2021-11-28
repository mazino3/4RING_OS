#
# 4RING_OS License: MIT
#
# Makefile for Applications 
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../defines.mk

INCL_SYS = ../include
INCL_LIBS = $(INCL_SYS)/libs
LIBS_INCL = ../libs/include
INCLUDE = $(INCL_SYS) -I $(INCL_LIBS) -I $(LIBS_INCL)
LIBC = ../libs/4libc/4libc.a

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I $(INCLUDE) -fdeclspec -fpack-struct \
	
LDFLAGS =

all: apps.o

c.o:
	$(CC) $(CFLAGS) -O -o $*.o $<

OBJS = hello.o 
	
apps.o: $(OBJS) # -z muldefs
	$(LD) -T apps_ld.scr -Map apps.map -M $(OBJS) $(LIBC) -z muldefs -o apps.o > apps.lnk
	objcopy -O binary -R .note -R .comment apps.o ../sys_out/apps
	objdump -D -z --disassembler-options=intel-mnemonic apps.o  > apps.dum

clean:
	rm -f *.o -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CC) -E -nostdinc -I $(INCLUDE) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	
# Dependencies

hello.o: hello.c ../include/libs/stdio.h ../libs/include/libc.h \
  ../libs/include/../4libc/include/stdio.h ../include/stddef.h \
  ../include/stdarg.h ../libs/include/../4libc/include/struct_FILE.h \
  ../libs/include/../4libc/include/libc_calls.h ../include/typedef.h \
  ../include/gdt.h ../include/sched.h ../include/io.h \
  ../include/gas_regs.h ../include/libs/lib_defs.h
