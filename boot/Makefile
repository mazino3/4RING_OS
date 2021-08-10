#
# 4RING_OS License: MIT
#
# Makefile for boot
#
# (C) Copyright 2021 Isa <isa@isoux.org>
 
include ../defines.mk

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I ../include -fdeclspec  -fpack-struct \
	-DMARCH=$(ARCH) -D$(BOOT_WAY)

ifeq ($(BOOT_WAY), GRUB2)
	LOAD_C = load_grub2.c
else
	LOAD_C = load_grub.c
endif

all:  load.o

load.o : $(LOAD_C)
	$(CC) $(CFLAGS) -O -o $*.o $(LOAD_C)

clean:
	rm -f *.o -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	
# Dependencies

load_grub.o: load_grub.c ../include/multiboot.h ../include/typedef.h \
  ../include/sys.h ../include/config.h ../include/gdt.h ../include/io.h \
  ../include/gas_regs.h
load_grub2.o: load_grub2.c ../include/multiboot.h ../include/typedef.h \
  ../include/sys.h ../include/config.h ../include/gdt.h
