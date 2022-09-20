// ceargv.c
//
// Time-stamp: <03/02/01 10:40:18 keuchel@lightning>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include "glob.h"
#include "errno.h"

#define GLOBFLAGS (GLOB_ERR | GLOB_NOCHECK | GLOB_NOESCAPE | GLOB_NOMAGIC)

// GLOB_BRACE seems to have a bug...
// GLOB_TILDE also

static int 
errfunc(const char *name, int err)
{
  fprintf(stderr, "glob:cannot access %s - %s\n", name, xcestrerror(err));
  return -1;
}

// argv size;
static int _size;

static void
argv_append(char *s)
{
  if(__argc >= _size - 1)
    {
      // expand argv
      _size *= 2;
      __argv = realloc(__argv, _size * sizeof(char *));
    }
  // we do not free the glob vector or command-line, so we do not dup
  __argv[__argc++] = s;
  __argv[__argc] = NULL;
}

// Simple Visual C _setargv replacement, does not handle nested
// quotes.. The original matches short names! So *.cpp matches #xxx.cpp#
// (which is XXXX~1.CPP as short name). This is very bad...

void
_setargv()
{
  int i, res;
  char *p;
  char *s;

  glob_t gbuf;
  gbuf.gl_pathc = 0;
  gbuf.gl_offs = 0;
  gbuf.gl_pathv = NULL;
  
  _size = 10;

  __argc = 0;
  __argv = malloc(_size * sizeof(char *));

  p = malloc(MAX_PATH);
  XCEGetModuleFileNameA(NULL, p, MAX_PATH);
  argv_append(p);

  // argv[0] is part of commandline
  s = p = (char *) XCEGetCommandLineA();

  // printf("Program is %s Commandline is %s\n", __argv[0], s);

  for(;;)
    {
      if(*p == 0)
	{
	  if(s != p)
	    {
	      glob(s, GLOBFLAGS, errfunc, &gbuf);
	      for(i = 0; i < gbuf.gl_pathc; i++)
		argv_append(gbuf.gl_pathv[i]);
	    }
	  break;
	}
      else if(*p == '"')
	{
	  argv_append(++p);
	  while(*p && *p != '"')
	    p++;
	  *p++ = 0;
	  s = p;
	}
      else if(*p == '\'')
	{
	  argv_append(++p);
	  while(*p && *p != '\'')
	    p++;
	  *p++ = 0;
	  s = p;
	}
      else if(*p == ' ' || *p == '\t')
	{
	  if(s != p)
	    {
	      *p++ = 0;
	      glob(s, GLOBFLAGS, errfunc, &gbuf);
	      for(i = 0; i < gbuf.gl_pathc; i++)
		argv_append(gbuf.gl_pathv[i]);
	    }
	  while(*p && (*p == '\t' || *p == ' '))
	    p++;
	  s = p;
	}
      else
	{
	  p++;
	}
    }

  // convert program name to unix style name
  for(p = __argv[0]; *p; p++)
    if(*p == '\\')
      *p = '/';
}
