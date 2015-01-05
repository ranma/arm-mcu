                      ARM Microcontroller Toolchain Notes

   This toolchain includes binutils, gcc, newlib, and gdb. It may be built
   for Linux, OpenBSD, and Windows (using Cygwin). The Linux, OpenBSD, and
   Windows releases are virtually identical in every respect. (This is the
   reason I created my own toolchain: I routinely flip back and forth
   among Linux, MacOS (with [1]Fink), OpenBSD, and Windows development
   environments.)

   My toolchain builds multilibs for the various Cortex-Mx devices,
   including hardware floating point for the Cortex-M4.
     __________________________________________________________________

   Questions or comments to Philip Munts [2]phil@munts.net

   $Id$

   I am available for custom system development (hardware and software) of
   products using ARM or other microcontrollers.

References

   1. http://www.finkproject.org/
   2. mailto:phil@munts.net
