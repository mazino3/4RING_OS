#
# 4RING_OS License: MIT
#
# Makefile for libs
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../defines.mk

#INCL_SYS = ../../include
#INCL = include/
# -I $(INCL_SYS) -I $(INCL) 

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin  -I ../include -fdeclspec  -fpack-struct \
	-D$(BOOT_WAY)
	
LDFLAGS =

all: 4libc/4libc lib_sys.a libs.o

c.o:
	$(CC) $(CFLAGS) -O -o $*.o $<
	
OBJS = libs_irq.o libs_task.o libs_sched.o ctype.o vsprintf.o glob_table.o 4libc/4libc.o
STATIC = printr.o
	
libs.o: $(OBJS) 
	$(LD) -T libs_ld.scr -Map libs.map -M $(OBJS) lib_sys.a -o libs.o > libs.lnk #-z muldefs
	objcopy -O binary -R .note -R .comment libs.o ../sys_out/libs
	objdump -D -z --disassembler-options=intel-mnemonic libs.o  > libs.dum
	
lib_sys.a: $(STATIC)
	$(AR) rcs lib_sys.a $(STATIC)
	sync
	
4libc/4libc:
	(cd 4libc; make)

clean:
	rm -f *.o -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile

# Dependencies

ctype.o: ctype.c ../include/ctype.h ../include/typedef.h
glob_table.o: glob_table.c ../include/libs/lib_defs.h \
  ../include/typedef.h
libs_irq.o: libs_irq.c ../include/gdt.h ../include/typedef.h \
  ../include/ldt.h ../include/task.h ../include/sched.h ../include/io.h \
  ../include/gas_regs.h ../include/core/sys_calls.h \
  ../include/libs/lib_sys.h
libs_task.o: libs_task.c ../include/gdt.h ../include/typedef.h \
  ../include/ldt.h ../include/task.h ../include/sys.h \
  ../include/config.h
printr.o: printr.c ../include/libs/lib_sys.h ../include/core/sys_calls.h \
  ../include/typedef.h ../include/gdt.h ../include/sched.h \
  ../include/io.h ../include/gas_regs.h
vsprintf.o: vsprintf.c ../include/stdarg.h ../include/string.h \
  ../include/typedef.h ../include/io.h ../include/gas_regs.h
