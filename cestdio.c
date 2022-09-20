// cestdio.c
//
// Time-stamp: <02/02/01 18:28:15 keuchel@lightning>

#include "celib.h"
#include "sys/stat.h"

// we need to fix path...

FILE *
xcefopen(const char *path, const char *mode)
{
  char newpath[MAX_PATH];
  wchar_t wpath[MAX_PATH];
  wchar_t wpathnew[MAX_PATH];
  wchar_t wmode[10];
  FILE *f;
  int fd;

  _initfds();

  // WinCE has no NUL!
  if(xcestricmp(path, "nul") == 0 ||
     xcestricmp(path, "null:") == 0 ||
     xcestricmp(path, "/dev/null") == 0)
    {
#if 0
      strcpy(newpath, "nul:");
#else
      strcpy(newpath, (char *) XCEGetUnixPath("/dev/null"));
#endif
    }
  else
    {
      strcpy(newpath, path);
    }

  MultiByteToWideChar(CP_ACP, 0, newpath, -1, wpath, COUNTOF(wpath));
  MultiByteToWideChar(CP_ACP, 0, mode, -1, wmode, COUNTOF(wmode));

  XCEFixPathW(wpath, wpathnew);

  //wprintf(L"xcefopen(%s)\n", wpathnew);

  if((f = _wfopen(wpathnew, wmode)) == NULL)
    {
      XCETRACEW((L"xcefopen(%s): oserr = %d\n", wpathnew, GetLastError()));

      return NULL;
    }

  // needed for xcefileno...
  fd = _getnewfd();

  _fdtab[fd].fd = fd;
  _fdtab[fd].type = XCE_FILE_TYPE_FILE;
  // seems to be os handle...
  _fdtab[fd].hFile = fileno(f);

  return f;
}

// there is only a wide version!

FILE *
xcefreopen(const char *path, const char *mode, FILE *stream)
{
  wchar_t wpath[MAX_PATH];
  wchar_t wpathnew[MAX_PATH];
  wchar_t wmode[10];

  MultiByteToWideChar(CP_ACP, 0, path, -1, wpath, COUNTOF(wpath));
  MultiByteToWideChar(CP_ACP, 0, mode, -1, wmode, COUNTOF(wmode));

  XCEFixPathW(wpath, wpathnew);

  //wprintf(L"xcefopen(%s)\n", wpathnew);

  return _wfreopen(wpathnew, wmode, stream);
}

void
xcefclose(FILE *f)
{
  int fd;

  for(fd = 0; fd < MAXFDS; fd++)
    {
      if(_fdtab[fd].hFile == fileno(f))
	{
	  _fdtab[fd].fd = -1;
	  break;
	}
    }
  fclose(f);
}

int
xcefileno(FILE *f)
{
  int fd;

  for(fd = 0; fd < MAXFDS; fd++)
    {
      if(_fdtab[fd].hFile == fileno(f))
	{
	  return _fdtab[fd].fd;
	}
    }

  fprintf(stderr, "Cannot get fileno for file %x\n", f);

  return -1;
}

void
xcerewind(FILE *f)
{
  fseek(f, 0L, SEEK_SET);
}

FILE *
xcefdopen(int fd, const char *mode)
{
  wchar_t wmode[10];

  if(fd < 0 || _fdtab[fd].fd == -1)
    return NULL;

  if(_fdtab[fd].type == XCE_FILE_TYPE_SOCKET)
    {
      errno = EINVAL;
      return NULL;
    }

  MultiByteToWideChar(CP_ACP, 0, mode, -1, wmode, COUNTOF(wmode));

  return _wfdopen(_fdtab[fd].hFile, wmode);
}

FILE *
xcepopen(const char *command, const char *mode)
{
  errno = ENOSYS;
  return NULL;
}

int
xcepclose(FILE *f)
{
  errno = ENOSYS;
  return -1;
}
