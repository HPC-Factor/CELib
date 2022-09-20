// makeunixpath.c
//
// Time-stamp: <29/01/01 00:47:33 keuchel@lightning>

#include "celib.h"

static char _unixdir[MAX_PATH] = "";

char *
XCEGetUnixPath(const char *path)
{
  static char newpath[MAX_PATH];

  if(_unixdir[0] == 0)
    XCEGetEnvironmentVariableFromRegA("UNIXROOTDIR", 
				      _unixdir, sizeof(_unixdir));

  // we expect that path is absolute...
  strcpy(newpath, _unixdir);
  strcat(newpath, path);

  return newpath;
}

