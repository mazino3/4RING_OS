
		/* signal properties */
#define _SIGABRT	22
#define _SIGMAX		32
		/* stdio properties */
#define _FNAMAX	64
#define _FOPMAX	16
#define _TNAMAX	16
		/* stdlib properties */
#define _EXFAIL	1
		/* storage alignment properties */
#define _AUPBND	1U
#define _ADNBND	1U
#define _MEMBND	1U
		/* time properties */

/* stdio.h standard header */

		/* macros */
#define NULL		_NULL
#define _IOFBF		0
#define _IOLBF		1
#define _IONBF		2
#define BUFSIZ		512
#define EOF			(-1)
#define FILENAME_MAX	_FNAMAX
#define FOPEN_MAX		_FOPMAX
#define L_tmpnam		_TNAMAX
#define TMP_MAX			32
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#define stdin		_Files[0]
#define stdout		_Files[1]
#define stderr		_Files[2]
		/* type definitions */

typedef struct {
	unsigned long _Off;	/* system dependent */
	} fpos_t;
typedef struct {
	unsigned short _Mode;
	short _Handle;
	unsigned char *_Buf, *_Bend, *_Next;
	unsigned char *_Rend, *_Rsave, *_Wend;
	unsigned char _Back[2], _Cbuf, _Nback;
	char *_Tmpnam;
	} FILE;
		/* declarations */

extern FILE *_Files[FOPEN_MAX];

/* xstdio.h internal header */
#include <stdarg.h>
#include <stdio.h>
		/* bits for _Mode in FILE */
#define _MOPENR	0x1
#define _MOPENW	0x2
#define _MOPENA	0x4
#define _MTRUNC	0x8
#define _MCREAT	0x10
#define _MBIN	0x20
#define _MALBUF	0x40
#define _MALFIL	0x80
#define _MEOF	0x100
#define _MERR	0x200
#define _MLBF	0x400
#define _MNBF	0x800
#define _MREAD	0x1000
#define _MWRITE	0x2000

		/* declarations */

/* _Files data object */
#include "xstdio.h"

/* standard error buffer */
static unsigned char ebuf[80];

/* the standard streams */
static FILE sin = {	/* standard input */
	_MOPENR, 0,
	NULL, NULL, &sin._Cbuf,
	&sin._Cbuf, NULL, &sin._Cbuf, };
static FILE sout = {	/* standard output */
	_MOPENW, 1,
	NULL, NULL, &sout._Cbuf,
	&sout._Cbuf, NULL, &sout._Cbuf, };
static FILE serr = {	/* standard error */
	_MOPENW|_MNBF, 2,
	ebuf, ebuf + sizeof (ebuf), ebuf,
	ebuf, NULL, ebuf, };

/* the array of stream pointers */
FILE *_Files[FOPEN_MAX] = {&sin, &sout, &serr};
/* type definitions */
typedef struct {
	union {
		long li;
		long double ld;
		} v;
	char *s;
	int n0, nz0, n1, nz1, n2, nz2, prec, width;
	size_t nchar;
	unsigned int flags;
	char qual;
	} _Pft;
typedef struct {
	int (*pfn)(void *, int);
	void *arg;
	va_list ap;
	const char *s;
	int nchar, nget, width;
	char noconv, qual, stored;
	} _Sft;
