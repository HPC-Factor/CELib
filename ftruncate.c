// xceftruncate.c
//
// Time-stamp: <03/01/01 21:06:58 keuchel@lightning>

#include "celib.h"

int
xceftruncate(int fd, unsigned long size)
{
  DWORD newpos;

  if(fd < 0 || _fdtab[fd].fd == -1)
    return -1;

  if ((newpos = SetFilePointer(_fdtab[fd].hFile, size, NULL, 
			       FILE_BEGIN)) == -1)
    {
      return -1;
    }

  if(!SetEndOfFile(_fdtab[fd].hFile))
    {
      return -1;
    }

  return 0;
}
