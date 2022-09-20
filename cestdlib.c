// cestdlib.c
//
// Time-stamp: <03/02/01 12:45:45 keuchel@lightning>

#include "celib.h"
#include "sys/stat.h"

// problems when used in a dll!
int __argc;
char **__argv;
char **environ;
char *_commandline;
int __umask;
char *__progname;

void
xceinit(const wchar_t *commandlinew)
{
  int len;
  char path[MAX_PATH];
  char *p;

#ifdef _MT
  _init_multithread();
#endif

#if 0
  // we use PWD variable later...
  XCEGetModuleFileNameA(NULL, path, sizeof(path));

  if((p = xcestrrchr(path, '\\')) != NULL)
    *p = 0;

  XCESetCurrentDirectoryA(path);
#endif

  if(commandlinew != NULL)
    {
      len = wcslen(commandlinew) + 1;
      _commandline = malloc(len);

      WideCharToMultiByte(CP_ACP, 0, 
			  commandlinew, -1, 
			  _commandline, len, 
			  NULL, NULL);
    }
  else
    {
      _commandline = xcestrdup("");
    }

  _setargv();
  _initenv();

  XCESetCurrentRootDirectoryA("\\");

  if((p = xcegetenv("PWD")) != NULL)
    XCESetCurrentDirectoryA(p);
  else
    XCESetCurrentDirectoryA("\\");

  __progname = __argv[0];
}

char *
xcegetcwd(char *buf, int size)
{
  char wd[MAX_PATH];

  XCEGetCurrentDirectoryA(sizeof(wd), wd);

  if(buf)
    {
      strncpy(buf, wd, size);
      return buf;
    }

  return xcestrdup(wd);
}

void
xceabort()
{
  XCEShowMessageA("Abort");
  DebugBreak();
  exit(1);
}

int 
xcesystem(const char *cmd)
{
  return XCEExecuteProcessA(cmd, TRUE, NULL);
}

int
xcelocking(int fd, int mode, int nbytes)
{
  return 0;
}

int
xceumask(int mask)
{
  int oldumask = __umask;

  __umask = mask;

  return oldumask;
}

int
xcemkdir(const char *dirname)
{
  BOOL res = XCECreateDirectoryA(dirname, NULL);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  return 0;
}

int
xcermdir(const char *dirname)
{
  BOOL res = XCERemoveDirectoryA(dirname);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  return 0;
}

int
xcerename(const char *old, const char *new)
{
  struct xcestat st;
  BOOL res;

  if(xcestat(new, &st) == 0)
    {
      if(xceremove(new) < 0)
	{
	  return -1;
	}
    }

  res = XCEMoveFileA(old, new);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());

      return -1;
    }

  return 0;
}

int
xceremove(const char *path)
{
  BOOL res = XCEDeleteFileA(path);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());

      return -1;
    }

  return 0;
}

int
xcechmod(const char *fname, int mode)
{
  DWORD dwAttr;

  if((dwAttr = XCEGetFileAttributesA(fname)) == 0xFFFFFFFF)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  if(mode & S_IWRITE)
    dwAttr &= ~(FILE_ATTRIBUTE_READONLY);
  else
    dwAttr |= (FILE_ATTRIBUTE_READONLY);

  if(XCESetFileAttributesA(fname, dwAttr) == FALSE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  return 0;
}

int
xceaccess(const char *fname, int mode)
{
  struct xcestat st;

  if(xcestat(fname, &st) < 0)
    return -1;

  return 0;
}

int
xceunlink(const char *fname)
{
  BOOL res = XCEDeleteFileA(fname);

  if(res == FALSE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }
  return 0;
}

int
xcegetpid()
{
  DWORD dwProcId = GetCurrentProcessId();

  return dwProcId & 0x7FFFFFFF;
}

int
xcechdir(const char *dirname)
{
  return XCESetCurrentDirectoryA(dirname) == TRUE;
}

char *
xcemktemp(char *template)
{
  char *p;
  int i;
  unsigned int pid;
  struct xcestat st;
  char *pt;

  pid = xcegetpid();

  if((p = xcegetenv("TMP")) == NULL)
    return NULL;

  for(pt = template; *pt && *pt != 'X'; pt++);

  for(i = 'a'; i < 'z'; i++)
    {
      sprintf(pt, "%c%05u", i, pid);
      if(xcestat(template, &st) < 0 && errno == ENOENT)
	return template;
    }

  return NULL;
}

// there must be other things changed, path search, for example...

int
xcechroot(const char *dirname)
{
#ifdef USE_CHROOT
  XCETrace("chroot(%s)\n", dirname);

  // this sets errno already...
  if(XCESetCurrentRootDirectoryA(dirname) == FALSE)
     return -1;

  return 0;
#else
  errno = ENOSYS;
  return -1;
#endif
}
