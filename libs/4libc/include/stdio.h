/*
 * 4RING_OS License: MIT
 *
 * 4libc/stdio.h
 *
 * Taken and edited from The GNU C Library.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */
/* Define ISO C stdio on top of C++ iostreams.
   Copyright (C) 1991-2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/*
 *	ISO C99 Standard: 7.19 Input/output	<stdio.h>
 */

#ifndef _STDIO_H
#define _STDIO_H	1


#include <stddef.h>
#include <stdarg.h>
#include "struct_FILE.h"

typedef struct _STRUC_FILE FILE;


/* The possibilities for the third argument to `setvbuf'.  */
#define _IOFBF 0		/* Fully buffered.  */
#define _IOLBF 1		/* Line buffered.  */
#define _IONBF 2		/* No buffering.  */

/* Default buffer size.  */
#define BUFSIZ 8192

/* The value returned by fgetc and similar functions to indicate the
   end of the file.  */
#define EOF (-1)

/* A non-array type containing all information needed to specify uniquely
 * every position within a file.
 */
typedef struct {
  unsigned long _Off;	/* system dependent */
} fpos_t;

/* 	Minimum number of files that can be open at once. */
#define FOPEN_MAX 3

/* 	Maximum length of a filename.  */
#define FILENAME_MAX 255

/* Maximum size of character array to hold ctermid() output. */
#define L_ctermid 20
/* Maximum size of character array to hold tmpnam() output.  */
#define L_tmpnam 20
#define TMP_MAX 0x1000

/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */
#ifdef __USE_GNU
# define SEEK_DATA	3	/* Seek to next data.  */
# define SEEK_HOLE	4	/* Seek to next hole.  */
#endif

/* Standard streams.  */
extern FILE *stdin;		/* Standard input stream.  */
extern FILE *stdout;	/* Standard output stream.  */
extern FILE *stderr;	/* Standard error output stream.  */
/* C89/C99 say they're macros.  Make them happy.  */
#define stdin stdin
#define stdout stdout
#define stderr stderr

/// Library Functions


/// Formatted Input/Output functions

/* Write formatted output to buf from argument list args. */
extern int vsprintf(char * buf, const char * fmt, va_list args);

/* Write formatted output to stdout. */
extern int printf(const char * fmt, ...);


#endif /* _STDIO_H  */
