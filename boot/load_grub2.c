/*
 * 4RING_OS
 *
 * load_grub2.c
 *
 * Multiboot load procedures for GRUB version 2
 *
 * The GRUB loader uses different selectors for code (0x10) and data
 * on its gdt table, than mine (0x08). This is reason for reinitialize
 * gdt descriptor selectors for CS to be 0x08 and DS to be 0x10.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <multiboot.h>
#include <typedef.h>
#include <sys.h>
#include <gdt.h>

void load(void);
void load_mods (u_int);
void copy_module(u_int, u_int, u_int);

#define HEADER_FLAGS	PAGE_ALIGN + MEMORY_INFO

__attribute__((section(".multiboot")))
u_long multi_head [] = {
	HEADER_MAGIC,
	HEADER_FLAGS,
	-(HEADER_MAGIC + HEADER_FLAGS)
};

l_long multib_gdt[] = {
	0x0000000000000000,	// NULL descriptor
	0x00c09a00000007ff,	// sel. 0x08 CODE 8Mb 0x08, base = 0
	0x00c09200000007ff	// sel. 0x10 DATA 8Mb
};

struct gdt_opcode_p {
	u_short size;		// GDT_MAX_ENT entries
	u_int * gdt;		// ptr of gdt table + core base
} mb_gdt_opc = { 3*8, ((u_int *) & multib_gdt[0]) };

struct idt_opcode_p {
	u_short size;	// 256 entries
	u_int * idt;	// ptr of idt table + core base
} static mb_idt_opc = {0, 0};

__naked_ void set_idt_gdt(void) {
__asm{
	lidt mb_idt_opc
	lgdt mb_gdt_opc
	ret
	}
}

u_char stack_space[8192] = {0};

/* Arguments for Long JMP */
struct ljmp_args {
	u_long func_ptr;
	u_short selector;
} jmp_args = {(u_long) load, CORE_CODE};

__naked_ void start(void) {
  __asm{
	cli				// block interrupts
	call set_idt_gdt
	// 8259 Programmable Interrupt Controller (PIC)
	// Copied from Linux 0.01
	xor eax, eax
	mov	al,0x11		// initialization sequence
	out	0x20,al		// send it to 8259A-1
	out	0xA0,al		// and to 8259A-2
	mov	al,0x20		// start of hardware int's (0x20)
	out	0x21,al
	mov	al,0x28		// start of hardware int's 2 (0x28)
	out	0xA1,al
	mov	al,0x04		// 8259-1 is master
	out	0x21,al
	mov	al,0x02		// 8259-2 is slave
	out	0xA1,al
	mov	al,0x01		// 8086 mode for both
	out	0x21,al
	out	0xA1,al
	mov	al,0xFF		// mask off all interrupts for now
	out	0x21,al
	out	0xA1,al
	// End of copied from Linux 0.01
	ljmp [jmp_args]	// needs this for set CS at CORE_CODE sel. (0x08)
  }
}

__naked_ void load(void) {
  __asm{
	lea eax, stack_space[8192]
	mov esp, eax
	push ebx						// contains multiboot info structure
	call load_mods
	lcall CORE_CODE, CORE_START		// Needs long call for changing CS
	hlt
  }
}

__inlinea_ static u_int reverse_val(u_char * val) {
  __asm{
	mov eax, val
  }
#if (MARCH == i386)
  __asm{
	ror ax, 8
	ror eax, 16
	ror ax, 8
    }
#else
  __asm{bswap eax}
#endif
}

void load_mods(u_int info_struc) {

  int i;
  u_int mod_size, cmd, out_addr;
  info_t *mb_info;
  module_t *mod, *cmd_line;

  mb_info = (info_t *)info_struc;

  for (i = 0, mod = (module_t *) mb_info->mods_addr;
	  i < mb_info->mods_count; i++, mod++)
	{
		mod_size = (unsigned)mod->mod_end - (unsigned)mod->mod_start;
		cmd_line = (module_t *)mod->cmdline;
		cmd = reverse_val((u_char *)cmd_line->mod_start);
		switch (cmd)
		{
		  case 'gdt_':
			out_addr = GDT_START; // 0x7f7000
			break;
		  case 'core':
		  	out_addr = CORE_START;
		  	break;
		  case 'devs':
			out_addr = DEVS_START;
			break;
		  case 'libs':
		  	out_addr = LIBS_START;
		  	break;
		  case 'usrs':
		  	out_addr = USERS_START;
		  	break;
		  case 'apps':
		  	out_addr = 0x700000;
		  	break;
		  default:
			out_addr = 0;
		}
		if (out_addr)
		  copy_module(mod_size >> 2, mod->mod_start, out_addr);
  }
}

void copy_module(u_int mod_size, u_int in_addr, u_int out_addr) {
  __asm{
	cld
	mov ecx, mod_size
	mov esi, in_addr
	mov edi, out_addr
	rep movsd
  }
}
