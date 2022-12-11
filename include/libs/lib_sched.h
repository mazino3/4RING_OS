/*
 * 4RING_OS License: MIT
 *
 * libs/lib_sched.h
 *
 * Defines and structures for libs schedule functions
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _LIB_SCHED_H
#define _LIB_SCHED_H


#include <typedef.h>

struct libs_sched_struct{
  u_char req_ptr;
  u_int value;
  int_f_int func;
};


#endif /* _LIB_SCHED_H */
