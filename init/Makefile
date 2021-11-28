#
# 4RING_OS License: MIT
#
# Makefile for init
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../defines.mk

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I ../include -fdeclspec  -fpack-struct

all: init.o main.o

c.o:
	$(CC) $(CFLAGS) -O -o $*.o $<
	
OBJS = init_32.o

init.o: $(OBJS)
	$(LD) -r -o init.o $(OBJS)
	(make dump)

main.o : main.c

dump:
	objdump -D -z --disassembler-options=intel-mnemonic init.o  > init.dmp
	
clean:
	rm -f *.o *.dmp -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	
# Dependencies

init_32.o: init_32.c ../include/typedef.h ../include/task.h \
  ../include/gdt.h ../include/tty.h ../include/termios.h
main.o: main.c ../include/typedef.h ../include/sys.h ../include/config.h \
  ../include/gdt.h ../include/core/page.h
