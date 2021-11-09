# [4RING_OS](https://www.isoux.org/blog/index.php?article2/4ring_os)

Hobby operating system for Intel processors from 386 and up. Engaged on all 4 rings with call gates, paging, segmentation and almost all hardware capabilities provided by this generation of processors.

For a self-taught person like me, who is still learning to coding, realizing an idea by writing from scratch is not an easy process at all. There is also a fear of failure and ridicule from genius people and professionals. But sincere desire and joy when you master the basic functionality of hardware at least a little, overcomes all obstacles. It is an even greater inspiration that this little work will mean at least something to someone. To be honest, I was seriously surprised when I didn't find a single specific example of code anywhere on the Internet where at least the most brief explanation of the functionality and interaction between the 4 protection rings on Intel processors is documented.

I embarked on an adventure to try to understand this by mostly reading INTEL's technical documentation and studying LINUX from the very first version 0.00. I have spent a lot of time since I started this adventure. I succeeded a bit and new ideas were born. I started writing ugly code mostly in assembler and slowly rewriting it in C language according to the progress of learning. At the lowest level, the code is written from scratch, at higher levels where "OS" works with devices, libraries and user programs, I insert and customize ingenious coding solutions from earlier versions of LINUX and GNU software.

Although this system would be strictly hardware-oriented and not portable, I still have a tendency for this mini "OS" to be similar to unix-like systems and to implement the POSIX standard in it as much as I can.

- [Motivation](https://www.isoux.org/blog/index.php?article3/motivation)
- [Basic Idea and assumptions](https://www.isoux.org/blog/index.php?article4/basic-idea-and-assumptions)
- [Development and tools](https://www.isoux.org/blog/index.php?article5/development-and-tools)

## Release 0.00

<p style="text-align: justify;">Nothing spectacular for the zero release.</p>
<p style="text-align: justify;">What can be noticed is that the characters are printed on the monitor when the keyboard is pressed until the buffer is full. What is actually happening on the system is a "complex" int&#1077;raction between rings 1 and 0 for now, which can be seen in the code.</p>
<p style="text-align: justify;">Every interrupts made by the keyboard is captured by a procedure that is actually a task in ring one. Value is written to the queue buffer, and a request for kernel processing is sent to the core via the call gate immediately. After the core scheduler transfers that execution to the ring it requested, the task schedule procedure in that ring processes the interrupt in its domain...</p>
<p style="text-align: justify;">So each ring has its own task scheduler...</p>
<p style="text-align: justify;">To make it easier to understand the functionality of the system and for myself as well, I decided to start with release 0.00 for easier monitoring and gradual introduction to the complexity of the system. That is why ring 2 (libs) have not been launched in this release yet.</p>
<p style="text-align: justify;">The system is successfully tested on emulators as well as on real machines. It is currently compiled for i486 and 8M of RAM (for my <span style="color: #ffff00;"><a href="https://i.pinimg.com/originals/2d/24/65/2d24653bf7a545a23526c9f00fef7e11.jpg"><span style="color: #ffff00;"><em>COMPAQ AERO 486 machine</em></span></a></span>).</p>
<p style="text-align: justify;">Before I decided on GRUB loaders, with a simply written loader, I could run the system on 64-bit machines as well.</p>
<p style="text-align: justify;">However, when GRUB loaders (which are even compiled for 32-bit machines) run on 64-bit machines the system crashes. For now, I am not able to solve this problem.

In order to be able to use the BOCHS gui debbuger, I had to install an older version because the default version of BOCHS has a problem with the gui debugger on UBUNTU 20.04. Sometimes the qemu emulator works great and sometimes it crashes, which confused me, but when I checked the code on a real machine it always worked even though the qemu showed the opposite. But my goal is not to adapt my code to the QEMU or other emulators, but to the real machines.</p>
<p>&#160;</p>
<h2>&#160;</h2>
<p>&#160;</p>
<p>&#160;</p>
