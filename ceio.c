// ceio.h
//
// Time-stamp: <02/02/01 18:25:24 keuchel@lightning>

#include <winsock.h>
#include "celib.h"
#include "sys/stat.h"

// On WinCE, sockets are int?

int _fmode;

_fdent_t _fdtab[MAXFDS];

void
_initfds()
{
  int i;

  static int fdsinitialized = 0;

  if(fdsinitialized)
    return;

  for(i = 0; i < MAXFDS; i++)
    _fdtab[i].fd = -1;

  // emacs handles fd=0 as special!
  _fdtab[0].fd = 0;
  _fdtab[1].fd = 1;
  _fdtab[2].fd = 2;

  fdsinitialized = 1;
}

int
_getnewfd()
{
  int i;

  for(i = 0; i < MAXFDS; i++)
    {
      if(_fdtab[i].fd == -1)
	return i;
    }

  return -1;
}

HANDLE
xceget_osfhandle(int fd)
{
  if(fd < 0 || _fdtab[fd].fd == -1 || fd > MAXFDS)
    return INVALID_HANDLE_VALUE;

  return _fdtab[fd].hFile;
}

int
xcecreat(const char *filename, int pmode)
{
  int fd;

  fd = xceopen(filename, _O_CREAT|_O_TRUNC|_O_RDWR, pmode);
  
  return fd;
}

int
xceopen(const char *path, int oflag, ...)
{
  va_list ap;
  int pmode;
  wchar_t wfname[MAX_PATH];
  char newpath[MAX_PATH];

  va_start(ap, oflag);
  pmode = va_arg(ap, int);
  va_end(ap);

  // WinCE has no NUL!
  if(xcestricmp(path, "nul") == 0 ||
     xcestricmp(path, "null:") == 0 ||
     xcestricmp(path, "/dev/null") == 0)
    {
#if 0
      // i thought it worked, but doesnt
      strcpy(newpath, "nul:");
#else
      strcpy(newpath, (char *) XCEGetUnixPath("/dev/null"));
#endif
    }
  else
    {
      strcpy(newpath, path);
    }

  MultiByteToWideChar(CP_ACP, 0, newpath, -1, wfname, COUNTOF(wfname));
  return xceopenw(wfname, oflag, pmode);
}

int
xceopenw(const wchar_t *fname, int oflag, ...)
{
  HANDLE hFile;
  DWORD fileaccess;
  DWORD fileshare; 
  DWORD filecreate;
  DWORD fileattrib;
  int pmode;
  va_list ap;
  int fd;

  va_start(ap, oflag);
  pmode = va_arg(ap, int);
  va_end(ap);

  _initfds();

  fileshare = FILE_SHARE_READ|FILE_SHARE_WRITE;
  fileattrib = FILE_ATTRIBUTE_NORMAL;

#if 0
  // only if creating new file...
  if(!(pmode & _S_IWRITE))
    fileattrib = FILE_ATTRIBUTE_READONLY;
#endif

  switch( oflag & (_O_RDONLY | _O_WRONLY | _O_RDWR) ) 
    {
    case _O_RDONLY:         /* read access */
      fileaccess = GENERIC_READ;
      break;
    case _O_WRONLY:         /* write access */
      fileaccess = GENERIC_WRITE;
      break;
    case _O_RDWR:           /* read and write access */
      fileaccess = GENERIC_READ | GENERIC_WRITE;
      break;
    default:                /* error, bad oflag */
      errno = EINVAL;
      return -1;
    }

  switch ( oflag & (_O_CREAT | _O_EXCL | _O_TRUNC) ) 
    {
    case 0:
    case _O_EXCL:                   // ignore EXCL w/o CREAT
      filecreate = OPEN_EXISTING;
      break;

    case _O_CREAT:
      filecreate = OPEN_ALWAYS;
      break;

    case _O_CREAT | _O_EXCL:
    case _O_CREAT | _O_TRUNC | _O_EXCL:
      filecreate = CREATE_NEW;
      break;

    case _O_TRUNC:
    case _O_TRUNC | _O_EXCL:        // ignore EXCL w/o CREAT
      filecreate = TRUNCATE_EXISTING;
      break;

    case _O_CREAT | _O_TRUNC:
      filecreate = CREATE_ALWAYS;
      break;

    default:
      // this can't happen ... all cases are covered
      errno = EINVAL;
      return -1;
    }

  if((hFile = XCECreateFileW(fname,
			     fileaccess,
			     fileshare,
			     NULL,
			     filecreate,
			     fileattrib,
			     NULL)) == INVALID_HANDLE_VALUE)
    {
      errno = _winerror2errno(GetLastError());

      XCETRACEW((L"CreateFile(%s): errno = %d oserr = %d\n",
		 fname, errno, GetLastError()));

      return -1;
    }

  fd = _getnewfd();

  _fdtab[fd].fd = fd;
  _fdtab[fd].type = XCE_FILE_TYPE_FILE;
  _fdtab[fd].hFile = hFile;

  if(oflag & _O_APPEND)
    {
      SetFilePointer(hFile, 0, NULL, FILE_END);
    }

  return fd;
}

int
xceclose(int fd)
{
  if(fd < 0 || _fdtab[fd].fd == -1)
    {
      errno = EBADF;
      return -1;
    }

  if(_fdtab[fd].type == XCE_FILE_TYPE_FILE)
    XCECloseHandle(_fdtab[fd].hFile);
  else if(_fdtab[fd].type == XCE_FILE_TYPE_SOCKET)
    closesocket((SOCKET) _fdtab[fd].hFile);

  _fdtab[fd].fd = -1;

  return 0;
}

int
xceread(int fd, void *buf, int size)
{
  DWORD dwRead;

  if(fd < 0 || _fdtab[fd].fd == -1)
    {
      errno = EBADF;
      return -1;
    }

  if(fd == 0)
    {
      int cc = fread(buf, size, 1, stdin);

      if(cc == EOF)
	return 0;
      return cc;
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_FILE)
    {
      if(XCEReadFile(_fdtab[fd].hFile, buf, size, &dwRead, NULL) == FALSE)
	{
	  fprintf(stderr, "ReadFile: %d\n", GetLastError());
	  return -1;
	}
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_FILE)
    {
      dwRead = recv((SOCKET) _fdtab[fd].hFile, buf, size, 0);
    }

  return dwRead;
}

int
xcewrite(int fd, void *buf, int size)
{
  DWORD dwWritten;

  if(fd < 0 || _fdtab[fd].fd == -1)
    {
      errno = EBADF;
      return -1;
    }

  if(fd == 1)
    {
      return fwrite(buf, size, 1, stdout);
    }
  else if(fd == 2)
    {
      return fwrite(buf, size, 1, stderr);
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_FILE)
    {
      if(XCEWriteFile(_fdtab[fd].hFile, buf, size, &dwWritten, NULL) == FALSE)
	{
	  fprintf(stderr, "WriteFile: %d\n", GetLastError());
	  return -1;
	}
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_SOCKET)
    {
      dwWritten = send((SOCKET) _fdtab[fd].hFile, buf, size, 0);
    }

  return dwWritten;
}

off_t
xcelseek(int fd, int off, int whence)
{
  off_t newpos;

  if(fd < 0 || _fdtab[fd].fd == -1)
    {
      errno = EBADF;
      return -1;
    }

  if ((newpos = SetFilePointer(_fdtab[fd].hFile, off, NULL, whence)) == -1)
    return -1;

  return newpos;
}

int
xceisatty(int fd)
{
  return 0;
}

int
xcesetmode(int fd, int mode)
{
  int i;

  if(fd < 0 || _fdtab[fd].fd == -1)
    {
      errno = EBADF;
      return -1;
    }

  return 0;
}

int
xcesetmodefile(FILE *f, int mode)
{
  return _setmode(f, mode);
}
