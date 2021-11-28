/*
 * 4RING_OS License: MIT
 *
 * hello.c
 *
 * Trial program to show basic printf functionality.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 *
 */

#include <stdio.h>

int main(void) {

  int i, a = 0xa, b = 0xb;
  i = printf("Hello World a = %d, b = %d !\n", a, b);
  printf("The previous has %d characters printed. \n",i);
  return 0;
}

