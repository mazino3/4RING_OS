/*
 * 4RING_OS License: MIT
 *
 * sched.h
 *
 * Defines and structures for schedule functions
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _SCHED_H
#define _SCHED_H

#include <gdt.h>
#include <io.h>

/* Schedule structure with pointers for circular buffer at core
 * ring (0)
 */
struct sched_buf_ptrs {
  unsigned char in_cur_p;	// Current input pointer
  unsigned char out_cur_p;	// Current output pointer
};

/* Schedule structure for requests from less privileged levels.
 * (rings 1,2,3)
 */
struct sched_req_struc {
  u_char req_ptr;			// Requested pointer at core ring
  u_int value;
  void_f_int func;
};

/* Parameters for call gate function CG_CORE_TX_IRQ */
#define DECL_IRQ 0xFF		// Declare or register irq request
#define CLR_IRQ  1			// Clear irq request


#endif /* _SCHED_H */
