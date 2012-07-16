/******************************************************************************/
/*                                                                            */
/*    Namespace clean, non-reentrant newlib system call function handlers     */
/*                                                                            */
/******************************************************************************/

// $Id$

static const char revision[] = "$Id$";

#include <cpu.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>

extern char __heap_start__[];	// Beginning of heap, address set by linker
extern char __heap_end__[];	// End of heap, address set by linker

static char *freespace;		// Pointer to free space (unclaimed heap area)

// Rudimentary heap area manager.  The dynamic memory allocator in newlib
// (malloc() and friends) calls this function to claim some or all of the
// free space allocated for heap by the linker.  The heap area is bounded
// by __heap_start__ and __heap_end__ which are set by the linker.  Note that
// the memory allocator will never release memory it has claimed.

char *_sbrk(size_t bytes)
{
  char  *base;

/* Initialize free space pointer if first time through. */

  if (!freespace)
  {
    freespace = __heap_start__;
    memset(__heap_start__, 0, __heap_end__ - __heap_start__);
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

// Basic I/O services

int _open(char *path, int flags, int mode)
{
  errno_r = 0;
  return device_open(path, flags, mode);
}

int _close(int fd)
{
  errno_r = 0;
  return device_close(fd);
}

long _read(int fd, void *dst, size_t size)
{
  errno_r = 0;
  return device_read(fd, (char *) dst, size);
}

long _write(int fd, void *src, size_t size)
{
  errno_r = 0;
  return device_write(fd, (char *) src, size);
}

// File system support services

int _fstat(int fd, struct stat *st)
{
  errno_r = 0;
  return device_stat(fd, st);
}

int _isatty(int fd)
{
  errno_r = 0;
  return device_isatty(fd);
}

int _link(char *old, char *new)
{
  errno_r = ENOSYS;
  return -1;
}

off_t _lseek(int fd, off_t pos, int whence)
{
  errno_r = 0;
  return device_seek(fd, pos, whence);
}

int _stat(char *file, struct stat *st)
{
  errno_r = 0;

  int fd = device_lookup(file);
  if (fd < 0) return fd;

  return _fstat(fd, st);
}

int _unlink(char *name)
{
  errno_r = ENOSYS;
  return -1;
}

// Basic multiprogramming services

int _execve(char *name, char **argv, char **env)
{
  errno_r = ENOSYS;
  return -1;
}

void _exit(int status)
{
  for (;;);
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
