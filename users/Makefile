#
# 4RING_OS License: MIT
#
# Makefile for users
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../defines.mk

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I ../include -fdeclspec  -fpack-struct \
	-D$(BOOT_WAY)
	
LDFLAGS =
LIBS = ../libs/lib_sys.a

all: users.o

c.o:
	$(CC) $(CFLAGS) -O -o $*.o $<
	
OBJS =  main_task.o user.o
	
users.o: $(OBJS)
	$(LD) -T users_ld.scr -Map users.map -M $(OBJS) $(LIBS) -z muldefs  -o users.o > users.lnk
	objcopy -O binary -R .note -R .comment users.o ../sys_out/users
	objdump -D -z --disassembler-options=intel-mnemonic users.o  > users.dum

clean:
	rm -f *.o -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	
# Dependencies

main_task.o: main_task.c ../include/typedef.h ../include/gdt.h \
  ../include/ldt.h ../include/task.h ../include/sys.h \
  ../include/config.h ../include/libs/lib_sys.h \
  ../include/core/sys_calls.h ../include/sched.h ../include/io.h \
  ../include/gas_regs.h ../include/rings.h
user.o: user.c ../include/typedef.h ../include/users/user.h
