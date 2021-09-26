/*
 * 4RING_OS License: MIT
 *
 * users/user.h
 *
 * Definitions for users SYS space.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _USER_H
#define _USER_H

#define UNAME_LEN 16     // max char length for user name
#define MAX_USRS  0x20   // max users on user space

struct user
{
  long uid;              // user ID
  long * proc_list;      // list of procedures used by user
  char uname[UNAME_LEN]; // user name
};
typedef struct user struc_user;

#endif /* _USER_H */

