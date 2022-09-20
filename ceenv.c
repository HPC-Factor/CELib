// ceenv.c
//
// Time-stamp: <01/01/01 13:55:05 keuchel@lightning>

#include "celib.h"

static int _envsize = 10;
static int _envcount = 0;

void
_appendenv(char *s)
{
  if(_envcount >= _envsize - 1)
    {
      // expand argv
      _envsize *= 2;
      environ = realloc(environ, _envsize * sizeof(char *));
    }
  // we do not free the glob vector or command-line, so we do not dup
  environ[_envcount++] = s;
  environ[_envcount] = NULL;
}

char *
_findenv(const char *name)
{
  int i;
  char *s, *p;

  for(i = 0; i < _envcount; i++)
    {
      s = environ[i];
      p = strchr(s, '=');

      if(strncmp(name, s, p - s) == 0)
	return p + 1;
    }
  return NULL;
}

int
_replaceenv(const char *str)
{
  int i;
  char *s, *p;

  for(i = 0; i < _envcount; i++)
    {
      s = environ[i];
      p = strchr(s, '=') + 1;

      if(strncmp(str, s, p - s) == 0)
	{
	  free(environ[i]);
	  environ[i] = (char *) str;

	  return 0;
	}
    }
  return -1;
}

void
_initenv()
{
  int res;
  char valname[126];
  char data[1024];
  char buf[1024];
  HKEY hKey;
  DWORD dwValSize = sizeof(valname);
  DWORD dwDataSize = sizeof(data);
  DWORD dwType;
  int idx = 0;

  environ = malloc(_envsize * sizeof(char *));
  environ[0] = NULL;

  if((res = XCERegOpenKeyExA(HKEY_LOCAL_MACHINE, "Environment", 0,
			     KEY_READ, &hKey)) != 0)
    {
      fprintf(stderr, "RegOpenKeyEx: %d\n", res);
      return;
    }

  while(1)
    {
      dwValSize = sizeof(valname);
      dwDataSize = sizeof(data);

      res = XCERegEnumValueA(hKey, idx++, valname, &dwValSize,
			     NULL, &dwType, data, &dwDataSize);

      if(res != 0)
	{
	  if(res != ERROR_NO_MORE_ITEMS)
	    fprintf(stderr, "RegEnumValue: %d\n", res);
	  break;
	}

      if(dwType != REG_SZ)
	continue;

      sprintf(buf, "%s=%s", valname, data);

      _appendenv(xcestrdup(buf));
    }

  XCERegCloseKey(hKey);
}

char *
xcegetenv(const char *name)
{
  char *p;

  return _findenv(name);
}

int
xceputenv(const char *s)
{
  char *p = xcestrdup(s);

  if(_replaceenv(p) < 0)
    _appendenv(p);

  return 0;
}
