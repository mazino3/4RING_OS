/*
 * 4RING_OS License: MIT
 *
 * tty.c
 *
 * The file is mostly taken from Linux 0.01
 *
 * Minimally implemented functionality for first show & try.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <tty.h>
#include <ctype.h>
#include <gdt.h>
#include <core/sys_calls.h>

extern void write_con(struct tty_struct * tty);

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

#define I_UCLC(tty)     _I_FLAG((tty),IUCLC)
#define I_NLCR(tty)     _I_FLAG((tty),INLCR)
#define I_CRNL(tty)     _I_FLAG((tty),ICRNL)
#define I_NOCR(tty)     _I_FLAG((tty),IGNCR)

#define O_POST(tty)     _O_FLAG((tty),OPOST)
#define O_NLCR(tty)     _O_FLAG((tty),ONLCR)
#define O_CRNL(tty)     _O_FLAG((tty),OCRNL)
#define O_NLRET(tty)	_O_FLAG((tty),ONLRET)
#define O_LCUC(tty)     _O_FLAG((tty),OLCUC)

struct tty_struct tty_tbl[1] = {
	{	{0,
		OPOST|ONLCR,	/* change outgoing NL to CRNL */
		0,
		ICANON | ECHO | ECHOCTL | ECHOKE,
		0,				/* console termios */
		INIT_C_CC},
		write_con,
		{0,0,0,0,""},	/* console read-queue */
		// {0,0,0,0,""}, /* console write-queue (originally)  */
		// Is changed for showing the initiate message.
		{0,527,0,0,"\r\n\
   _   _    _____   _____  _   _   _____          ____    _____\r\n\
  | | | |  |  __ \\ |_    || \\ | | / ____)        / __ \\  / ____)\r\n\
  | |_| |_ | |__) |  | |  |  \\| || |  __   ver. | |  | || {___\r\n\
  |___   _||  _  /   | |  | . ` || | |_ |       | |  | | \\__  \\\r\n\
      | |  | | \\ \\  _| |_ | |\\  || |__| |  0.00 | |__| | ___)  }\r\n\
      |_|  |_|  \\_||_____||_| \\_| \\_____| ______ \\____/ {_____/\r\n\
                                         |______|\r\n\
\r\n"},	/* cons. write-queue (show init message) */
		{0,0,0,0,""}	/* console sec-queue */
	}
};

struct tty_queue * tty_tbl_list[2] = {
	&tty_tbl[0].read_q, &tty_tbl[0].write_q
};

void prepare(struct tty_struct * tty)
{
	signed char c;

	while (!EMPTY(tty->read_q) && !FULL(tty->sec_q)) {
		GETCH(tty->read_q,c);
		if (c==13)
			if (I_CRNL(tty))
				c=10;
			else if (I_NOCR(tty))
				continue;
			else ;
		else if (c==10 && I_NLCR(tty))
			c=13;
		if (L_CANON(tty)) {
			if (c==ERASE_CHAR(tty)) {
				if (EMPTY(tty->sec_q) ||
				   (c=LAST(tty->sec_q))==10 ||
				   c==EOF_CHAR(tty))
					continue;
				if (L_ECHO(tty)) {
					if (c<32)
						PUTCH(127,tty->write_q);
					PUTCH(127,tty->write_q);
					tty->write(tty);
				}
				DEC(tty->sec_q.head); // pazi
				continue;
			}/*
			if (c==STOP_CHAR(tty)) {
				tty->stopped=1;
				continue;
			}
			if (c==START_CHAR(tty)) {
				tty->stopped=0;
				continue;
			} */
		}
		/* if (!L_ISIG(tty)) {
			if (c==INTR_CHAR(tty)) {
				tty_intr(tty,SIGINT);
				continue;
			} 
		} */
		if (c==10 || c==EOF_CHAR(tty))
			tty->sec_q.data++;
		if (L_ECHO(tty)) {
			if (c==10) {
				PUTCH(10,tty->write_q);
				PUTCH(13,tty->write_q);
			} else if (c<32) {
				if (L_ECHOCTL(tty)) {
					PUTCH('^',tty->write_q);
					PUTCH(c+64,tty->write_q);
				}
			} else
				PUTCH(c,tty->write_q);
			 tty->write(tty);
		}
		PUTCH(c,tty->sec_q);
	}
}

int tty_write(unsigned channel, char * buf, int nr)
{
	static int cr_flag=0;
	struct tty_struct * tty;
	char c, *b=buf;

	if (channel>2 || nr<0) return -1;
	tty = tty_tbl; // + channel;
	while (nr>0) {
		while (nr>0 && !FULL(tty->write_q)) {
			c = *b;
			if (O_POST(tty)) {
				if (c=='\r' && O_CRNL(tty)){
					c='\n';
				}
				if (c=='\n') {
					PUTCH(13,tty->write_q);
				}
			}
			b++; nr--;
			cr_flag = 0;
			PUTCH(c,tty->write_q);
		}
		tty->write(tty);
	}
	return (b-buf);
}

__naked_ void _tty_write(void){
  __asm{
	mov ecx, ds
	push ecx
	sub esp, 0x10
	mov cx, DEVS_DATA
	mov ds, ecx

	push [esp + 0x24]
	push [esp + 0x24]
	push [esp + 0x24]
	call tty_write

	add esp, 28
	pop ecx
	mov ds, cx
	retf 3 * 4
  }
}


void tty_interrupt(int tty)
{
	prepare(tty_tbl + tty);
}

