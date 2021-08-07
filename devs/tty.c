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

extern void write_con(struct tty_struct * tty);

struct tty_struct tty_tbl[1] = {
	{	{0,
		OPOST|ONLCR,	/* change outgoing NL to CRNL */
		0,
		ICANON | ECHO | ECHOCTL | ECHOKE,
		0,				/* console termios */
		INIT_C_CC},
		{0,0,0,0,""},	/* console read-queue */
		// {0,0,0,0,""}, /* console write-queue (originally)  */
		// Is changed for showing the initiate message.
		{0,0xA,0,0,"4RING_OS\r\n"},	/* cons. write-queue (show init message) */
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
		//if (I_UCLC(tty))
			//c=tolower(c);
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
					// tty->write(tty);	// right implementation
					write_con(tty);		// temporary
				}
				// DEC(tty->sec_q.head);
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
			// tty->write(tty);	// right implementation
			write_con(tty);		// temporary
		}
		//PUTCH(c,tty->sec_q);
	}
}

void tty_interrupt(int tty)
{
	prepare(tty_tbl + tty);
}

