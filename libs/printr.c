/*
 * 4RING_OS License: MIT
 *
 * libs/printr.c
 *
 * Part of system lib static lib_sys.a for 4RING_OS
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 *
 */

#include <libs/lib_sys.h>

int printr(const char *fmt, ...)
{
  int i;
    i = __printr(&fmt);
    return i;
}
