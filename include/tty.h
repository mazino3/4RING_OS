/*
 * 4RING_OS License: MIT
 *
 * tty.h
 *
 * The file is mostly taken from Linux 0.01
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */
 
#ifndef _TTY_H
#define _TTY_H

#include <termios.h>

#define TTY_BUF_SIZE 1024

struct tty_queue {
	unsigned long data;
	unsigned long head;
	unsigned long tail;
	unsigned int * proc_list;
	char buf[TTY_BUF_SIZE];
};

#define _L_FLAG(tty,f)	((tty)->termios.c_lflag & f)
#define _I_FLAG(tty,f)	((tty)->termios.c_iflag & f)
#define _O_FLAG(tty,f)	((tty)->termios.c_oflag & f)

#define L_CANON(tty)	_L_FLAG((tty),ICANON)
#define L_ISIG(tty)		_L_FLAG((tty),ISIG)
#define L_ECHO(tty)		_L_FLAG((tty),ECHO)
#define L_ECHOE(tty)	_L_FLAG((tty),ECHOE)
#define L_ECHOK(tty)	_L_FLAG((tty),ECHOK)
#define L_ECHOCTL(tty)	_L_FLAG((tty),ECHOCTL)
#define L_ECHOKE(tty)	_L_FLAG((tty),ECHOKE)

#define I_UCLC(tty)	_I_FLAG((tty),IUCLC)
#define I_NLCR(tty)	_I_FLAG((tty),INLCR)
#define I_CRNL(tty)	_I_FLAG((tty),ICRNL)
#define I_NOCR(tty)	_I_FLAG((tty),IGNCR)

#define O_POST(tty)	_O_FLAG((tty),OPOST)
#define O_NLCR(tty)	_O_FLAG((tty),ONLCR)
#define O_CRNL(tty)	_O_FLAG((tty),OCRNL)
#define O_NLRET(tty)	_O_FLAG((tty),ONLRET)
#define O_LCUC(tty)	_O_FLAG((tty),OLCUC)

#define INC(a) ((a) = ((a)+1) & (TTY_BUF_SIZE-1))
#define DEC(a) ((a) = ((a)-1) & (TTY_BUF_SIZE-1))
#define EMPTY(a) ((a).head == (a).tail)
#define LEFT(a) (((a).tail-(a).head-1)&(TTY_BUF_SIZE-1))
#define LAST(a) ((a).buf[(TTY_BUF_SIZE-1)&((a).head-1)])
#define FULL(a) (!LEFT(a))
#define CHARS(a) (((a).head-(a).tail)&(TTY_BUF_SIZE-1))
#define GETCH(queue,c) (void)({c=(queue).buf[(queue).tail];INC((queue).tail);})
#define PUTCH(c,queue) (void)({(queue).buf[(queue).head]=(c);INC((queue).head);})

#define EOF_CHAR(tty) ((tty)->termios.c_cc[VEOF])
#define INTR_CHAR(tty) ((tty)->termios.c_cc[VINTR])
#define STOP_CHAR(tty) ((tty)->termios.c_cc[VSTOP])
#define START_CHAR(tty) ((tty)->termios.c_cc[VSTART])
#define ERASE_CHAR(tty) ((tty)->termios.c_cc[VERASE])

#define INIT_C_CC "\003\034\177\025\004\0\1\0\021\023\031\0\022\017\027\026\0"

struct tty_struct {
	struct termios termios;
	struct tty_queue read_q;
	struct tty_queue write_q;
	struct tty_queue sec_q;
};

extern struct tty_struct tty_tbl[1];
extern struct tty_queue * tty_tbl_list[2];

extern void tty_interrupt(int tty);

extern long is_devs_task;

#define _data 0
#define _head 4
#define _tail 8
#define _proc_list 12
#define _buf  16

#endif /* _TTY_H */

