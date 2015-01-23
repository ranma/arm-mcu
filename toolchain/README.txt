                      ARM Microcontroller Toolchain Notes

   This toolchain includes binutils, gcc, newlib, and gdb. It may be built
   for Linux, OpenBSD, and Windows (using Cygwin). The Linux, OpenBSD, and
   Windows releases are virtually identical in every respect. (This is the
   reason I created my own toolchain: I routinely flip back and forth
   among Linux, MacOS (with [1]Fink), OpenBSD, and Windows development
   environments.) It is even possible to build the toolchain as a
   [2]Canadian Cross to run on a small Linux system such as the
   [3]Raspberry Pi. See [4]Makefile.rpi for an example.

   My toolchain builds multilibs for the various Cortex-Mx devices,
   including hardware floating point for the Cortex-M4.
     __________________________________________________________________

   Questions or comments to Philip Munts [5]phil@munts.net

   $Id$

   I am available for custom system development (hardware and software) of
   products using ARM or other microcontrollers.

References

   1. http://www.finkproject.org/
   2. http://en.wikipedia.org/wiki/Cross_compiler#Canadian_Cross
   3. http://www.raspberrypi.org/
   4. http://tech.munts.com/MCU/Frameworks/ARM/toolchain/Makefile.rpi
   5. mailto:phil@munts.net
