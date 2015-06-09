/******************************************************************************/
/*                                                                            */
/*    Namespace clean, non-reentrant newlib system call function handlers     */
/*                                                                            */
/******************************************************************************/

// $Id$

// Copyright (C)2013-2015, Philip Munts, President, Munts AM Corp.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

static const char revision[] = "$Id$";

#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>

#include <cpu.h>

// Rudimentary heap area manager.  The dynamic memory allocator in newlib
// (malloc() and friends) calls this function to claim some or all of the
// free space allocated for heap by the linker.  The heap area is bounded
// by __heap_start__ and __heap_end__ which are set by the linker.  Note that
// the memory allocator will never release memory it has claimed.

extern char __heap_start__[];	// Beginning of heap, address set by linker
extern char __heap_end__[];	// End of heap, address set by linker

static char *freespace;		// Pointer to free space (unclaimed heap area)

char *_sbrk(size_t bytes)
{
  char  *base;

/* Initialize free space pointer if first time through. */

  if (!freespace)
  {
    freespace = __heap_start__;
  }

/* Check for enough free space available */

  if (freespace + bytes > __heap_end__)
  {
    errno_r = ENOMEM;
    return NULL;
  }

  base = freespace;	// Get address of free space

  freespace += bytes;	// Claim heap area memory / Advance free space pointer

  errno_r = 0;
  return base;		// Return pointer to claimed heap area
}

// Basic I/O services have been moved to device.c

// File system support services -- Mostly unimplemented stubs

int _link(char *old, char *new)
{
  errno_r = ENOSYS;
  return -1;
}

int _stat(char *file, struct stat *st)
{
  errno_r = ENOSYS;
  return -1;
}

int _unlink(char *name)
{
  errno_r = ENOSYS;
  return -1;
}

// Basic multiprogramming services -- Mostly unimplemented stubs

int _execve(char *name, char **argv, char **env)
{
  errno_r = ENOSYS;
  return -1;
}

void _exit(int status)
{
  for (;;)
  {
    __WFI();
  }
}

int _fork(void)
{
  errno_r = ENOSYS;
  return -1;
}

pid_t _getpid(void)
{
  errno_r = 0;
  return 1;
}

int _kill(int pid, int sig)
{
  errno_r = ENOSYS;
  return -1;
}

int _times(struct tms *buf)
{
  errno_r = ENOSYS;
  return -1;
}

int _wait(int *status)
{
  errno_r = ENOSYS;
  return -1;
}
