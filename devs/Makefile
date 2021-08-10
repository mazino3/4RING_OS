#
# 4RING_OS License: MIT
#
# Makefile for devs
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../defines.mk

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I ../include -fdeclspec  -fpack-struct \
	-D$(BOOT_WAY)
	
LDFLAGS =

all: devs.o

c.o:
	$(CC) $(CFLAGS) -O -o $*.o $<
	
OBJS =  keyboard.o tty.o devs_task.o devs_sched.o devs_irq.o console.o
	
devs.o: $(OBJS) 
	$(LD) -T devs_ld.scr -Map devs.map -M $(OBJS) -o devs.o > devs.lnk
	objcopy -O binary -R .note -R .comment devs.o ../sys_out/devs
	#objdump -D -z --disassembler-options=intel-mnemonic devs.o  > devs.dum

clean:
	rm -f *.o -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile

# Dependencies

console.o: console.c ../include/typedef.h ../include/gdt.h \
  ../include/tty.h ../include/termios.h ../include/io.h \
  ../include/gas_regs.h ../include/display.h
devs_irq.o: devs_irq.c ../include/gdt.h ../include/typedef.h \
  ../include/sched.h
devs_sched.o: devs_sched.c ../include/typedef.h ../include/gdt.h \
  ../include/sched.h
devs_task.o: devs_task.c ../include/gdt.h ../include/typedef.h \
  ../include/ldt.h ../include/task.h ../include/sys.h \
  ../include/config.h
keyboard.o: keyboard.c ../include/typedef.h ../include/gdt.h \
  ../include/idt.h ../include/gates.h ../include/io.h \
  ../include/gas_regs.h ../include/tty.h ../include/termios.h \
  ../include/interupt.h ../include/task.h
tty.o: tty.c ../include/typedef.h ../include/ctype.h ../include/gdt.h \
  ../include/idt.h ../include/gates.h ../include/io.h \
  ../include/gas_regs.h ../include/tty.h ../include/termios.h \
  ../include/task.h
