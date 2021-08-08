# [4RING_OS](https://www.isoux.org/blog/index.php?article2/4ring_os)

[![Downloads](https://img.shields.io/github/downloads/isoux/4RING_OS/total)](https://github.com/isoux/4RING_OS/releases/latest)

Hobby operating system for Intel processors from 386 and up. Engaged on all 4 rings with call gates, paging, segmentation and almost all hardware capabilities provided by this generation of processors.

- [Motivation](https://www.isoux.org/blog/index.php?article3/motivation)
- [Basic Idea and assumptions](https://www.isoux.org/blog/index.php?article4/basic-idea-and-assumptions)
- [Development and tools](https://www.isoux.org/blog/index.php?article5/development-and-tools)

## Release 0.00

Nothing spectacular for the zero release.

What can be noticed is that the characters are printed on the monitor when the keyboard is pressed until the buffer is full. What is actually happening on the system is a "complex" intеraction between rings 1 and 0 for now, which can be seen in the code.

Every interrupts made by the keyboard is captured by a procedure that is actually a task in ring one. Value is written to the queue buffer, and a request for kernel processing is sent to the core via the call gate immediately. After the core scheduler transfers that execution to the ring it requested, the task schedule procedure in that ring processes the interrupt in its domain...

So each ring has its own task scheduler...

To make it easier to understand the functionality of the system and for myself as well, I decided to start with release 0.00 for easier monitoring and gradual introduction to the complexity of the system. That is why rings 2 and 3 have not been launched in this release yet.

The system is successfully tested on emulators as well as on real machines. It is currently compiled for i486 and 8M of RAM (for my COMPAQ AERO 486 machine). The only obstacle to compiling and testing on i386 machines, for now, is one instruction in the code that starts with version i486: BSWAP which is located in the files /boot/load_grub.c and load_grub2.c and is explained in detail there. I will solve this problem soon.

Before I decided on GRUB loaders, with a simply written loader, I could run the system on 64-bit machines as well.

However, when GRUB loaders (which are even compiled for 32-bit machines) run on 64-bit machines the system crashes. For now, I am not able to solve this problem.
