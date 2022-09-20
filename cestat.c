// cestat.c
//
// Time-stamp: <02/02/01 18:25:14 keuchel@lightning>

#include "celib.h"
#include "sys/stat.h"
#include "time.h"

extern _fdent_t _fdtab[];

int 
xcefstat(int fd, struct xcestat *st)
{
  BY_HANDLE_FILE_INFORMATION fi;
  SYSTEMTIME SystemTime;
  FILETIME LocalFTime;
  int permission = 0;
  HANDLE hFile;

  if(fd < 0 || _fdtab[fd].fd == -1)
    return -1;

  hFile = _fdtab[fd].hFile;

  memset(st, 0, sizeof(struct xcestat));
  
  XCEGetFileInformationByHandle(hFile, &fi);

  st->st_size = fi.nFileSizeLow;
  st->st_mode = _S_IFREG;

  if(fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    st->st_mode = _S_IFDIR;

  permission |= _S_IREAD;

  if(!(fi.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
    permission |= _S_IWRITE;

  if(fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    permission |= _S_IEXEC;

  st->st_mode |= permission | (permission >> 3) | (permission >> 6);

  FileTimeToLocalFileTime(&fi.ftLastWriteTime, &LocalFTime);
  FileTimeToSystemTime(&LocalFTime, &SystemTime);
  
  st->st_mtime = __loctotime_t( SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				-1 );

  FileTimeToLocalFileTime(&fi.ftCreationTime, &LocalFTime);
  FileTimeToSystemTime(&LocalFTime, &SystemTime);
  
  st->st_ctime = __loctotime_t( SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				-1 );

  FileTimeToLocalFileTime(&fi.ftLastAccessTime, &LocalFTime);
  FileTimeToSystemTime(&LocalFTime, &SystemTime);

  st->st_atime = __loctotime_t( SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				-1 );

  if(st->st_atime == 0) 
    st->st_atime = st->st_mtime;
  if (st->st_ctime == 0) 
    st->st_ctime = st->st_mtime;

  if(fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    st->st_nlink = 2;
  else
    st->st_nlink = fi.nNumberOfLinks;

  // Only low seems to be used...
  //printf("Low: %d High: %d\n", fi.nFileIndexLow, fi.nFileIndexHigh);

  st->st_rdev = 1;
  st->st_ino = fi.nFileIndexLow;

  return 0;
}

int 
xcestat_old(const char *fname, struct xcestat *st)
{
  int fd;
  int res;
  DWORD dwAttr;
  char *p;
  
  if((dwAttr = XCEGetFileAttributesA(fname)) == 0xFFFFFFFF)
    {
      errno = _winerror2errno(GetLastError());
      return -1;
    }

  if(dwAttr & FILE_ATTRIBUTE_DIRECTORY)
    {
      int permission = 0;

      memset(st, 0, sizeof(struct xcestat));

      st->st_mode = _S_IFDIR;

      permission |= _S_IREAD;
      permission |= _S_IWRITE;
      permission |= _S_IEXEC;

      st->st_mode |= permission | (permission >> 3) | (permission >> 6);

      st->st_nlink = 2;
      st->st_size = 1024;

      res = 0;
    }
  else
    {
      // this fills in inode correctly...
      if((fd = xceopen(fname, _O_RDONLY, 0)) < 0)
	return -1;

      res = xcefstat(fd, st);

      xceclose(fd);
    }

  return res;
}

int 
xcestat(const char *path, struct xcestat *st)
{
  WIN32_FIND_DATAA fd;
  HANDLE hFind;
  SYSTEMTIME SystemTime;
  FILETIME LocalFTime;
  int permission = 0;
  char fname[MAX_PATH];

  memset(st, 0, sizeof(struct xcestat));

  //printf("stat: %s\n", path);

  XCEFixPathA(path, fname);

  if(strcmp(fname, "\\") == 0 || strcmp(fname, "/") == 0)
    {
      st->st_size = 1024;
      st->st_mode = S_IFDIR;
      permission = S_IREAD|S_IWRITE|S_IEXEC;

      st->st_mode |= permission | (permission >> 3) | (permission >> 6);
      return 0;
    }

  if((hFind = XCEFindFirstFileA(fname, &fd)) == INVALID_HANDLE_VALUE)
    {
      DWORD dwError = GetLastError();

      if(dwError == ERROR_NO_MORE_FILES)
	errno = ENOENT;
      else
	errno = _winerror2errno(GetLastError());

      XCETRACE(("stat: FindFile(%s): errno = %d oserr = %d\n",
		fname, errno, GetLastError()));

      return -1;
    }

  if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      st->st_size = 1024;
    }
  else
    {
      st->st_size = fd.nFileSizeLow;
    }

  if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      st->st_mode = _S_IFDIR;
    }
  else
    {
      st->st_mode = _S_IFREG;
    }

  permission |= _S_IREAD;

  if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
    permission |= _S_IWRITE;

  if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    permission |= _S_IEXEC;

  st->st_mode |= permission | (permission >> 3) | (permission >> 6);

  FileTimeToLocalFileTime(&fd.ftLastWriteTime, &LocalFTime);
  FileTimeToSystemTime(&LocalFTime, &SystemTime);
  
  st->st_mtime = __loctotime_t( SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				-1 );

  FileTimeToLocalFileTime(&fd.ftCreationTime, &LocalFTime);
  FileTimeToSystemTime(&LocalFTime, &SystemTime);
  
  st->st_ctime = __loctotime_t( SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				-1 );

  FileTimeToLocalFileTime(&fd.ftLastAccessTime, &LocalFTime);
  FileTimeToSystemTime(&LocalFTime, &SystemTime);

  st->st_atime = __loctotime_t( SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				-1 );

  if(st->st_atime == 0) 
    st->st_atime = st->st_mtime;
  if (st->st_ctime == 0) 
    st->st_ctime = st->st_mtime;

  if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    st->st_nlink = 2;
  else
    st->st_nlink = 1;

  st->st_rdev = 1;
  st->st_ino = fd.dwReserved0; // is fd.dwOID;

  FindClose(hFind);

  return 0;
}

