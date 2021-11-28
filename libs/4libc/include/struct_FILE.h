/*
 * 4RING_OS License: MIT
 *
 * struct_FILE.h
 *
 * Defines for FILE structure
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.
*/

#ifndef __struct_FILE
#define __struct_FILE

typedef long off_t;
//typedef unsigned int fpos_t;

struct _STRUC_FILE;
struct marker;

typedef void lock_t;
typedef struct _STRUC_FILE FILE;

/* A streammarker remembers a position in a buffer. */
struct marker {
  struct marker *_next;
  FILE *_sbuf;
  int _pos;
};

/* The tag name of this struct is _STRUC_FILE to preserve historic
   C++ mangled names for functions taking FILE* arguments.
   That name should not be used in new code.  */
struct _STRUC_FILE
{
  int _flags;		/* High-order word is MAGIC; rest is flags. */
  /* The following pointers correspond to the C++ streambuf protocol. */
  char *read_ptr;	/* Current read pointer */
  char *read_end;	/* End of get area. */
  char *read_base;	/* Start of putback+get area. */
  char *write_base;	/* Start of put area. */
  char *write_ptr;	/* Current put pointer. */
  char *write_end;	/* End of put area. */
  char *buf_base;	/* Start of reserve area. */
  char *buf_end;	/* End of reserve area. */
  /* The following fields are used to support backing up and undo. */
  char *save_base; /* Pointer to start of non-current get area. */
  char *backup_base;  /* Pointer to first valid character of backup area */
  char *save_end; /* Pointer to end of non-current get area. */

  struct marker *markers;
  struct _STRUC_FILE *chain;

  int fileno;
  int flags2;
  off_t old_offset; /* This used to be _offset but it's too small.  */

  /* 1+column number of pbase(); 0 is unknown. */
  unsigned short cur_column;
  signed char vtable_offset;
  char shortbuf[1];

  lock_t *lock;
};

#endif /* __struct_FILE */
