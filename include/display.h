/*
 * 4RING_OS License: MIT
 *
 * display.h
 *
 * IBM PC display definitions
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

/* Color attributes for foreground text */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#define	FG_BLACK	0x0
#define	FG_BLUE		0x1
#define	FG_GREEN	0x2
#define	FG_CYAN		0x3
#define	FG_RED		0x4
#define	FG_MAGEN	0x5
#define	FG_BROWN	0x6
#define	FG_LGREY	0x7
#define	FG_DGREY	0x8
#define	FG_LBLUE	0x9
#define	FG_LGREEN	0xa
#define	FG_LCYAN	0xb
#define	FG_LRED		0xc
#define	FG_LMAG		0xd
#define	FG_YELLOW	0xe
#define	FG_WHITE	0xf
#define	FG_BLINK	0x80

/* Color attributes for text background */

#define	BG_BLACK	0x00
#define	BG_BLUE		0x10
#define	BG_GREEN	0x20
#define	BG_CYAN		0x30
#define	BG_RED		0x40
#define	BG_MAGEN	0x50
#define	BG_BROWN	0x60
#define	BG_LGREY	0x70
#define	BG_GREY		0x80
#define	BG_LBLUE	0x90

/* Monochrome attributes for foreground text */

#define	FG_UNDERLINE	0x01
#define	FG_INTENSE		0x08

/* Monochrome attributes for text background */

#define	BG_INTENSE		0x10

#endif /* _DISPLAY_H */
