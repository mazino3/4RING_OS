#
# 4RING_OS License: MIT
#
# Common make definitions
#
# (C) Copyright 2021 Isa <isa@isoux.org>

CC = clang
LD = ld -m elf_i386
ARCH = i486
CPP	= $(CC) -E -nostdinc -I ../include

CP := cp
RM := rm -rf
MKDIR := mkdir -pv

CORE = sys_out/core
DEVS = sys_out/devs
LIBS = sys_out/libs
USRS = sys_out/users

CORE_RAM = core
DEVS_RAM = devs
LIBS_RAM = libs
USRS_RAM = usrs
GDT_RAM = gdt_

LOAD = sys_out/sys_load.o
GDT = core/gdt_mem.bin
SYS = 4ring_os
SYS_LOAD = load
IMG_FILES = img/*
CFG = boot/grub.cfg

IMG_PATH := img
ISO_PATH := iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub

# GRUB or GRUB2 : One of these must be defined!
export BOOT_WAY = GRUB2

