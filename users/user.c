/*
 * 4RING_OS License: MIT
 *
 * user.c
 *
 * Memory allocators for keeping all about users.
 * Procedures for creating and tracking users.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include<typedef.h>
#include<users/user.h>

/* Table of user structure elements */
struc_user user_tlb[MAX_USRS] = {
	{ 0, NULL, "root"},
};
/* The current user */
u_int cur_user = 0;
