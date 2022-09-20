// wsdb.c - missing netdb stuff for windows ce
//
// Time-stamp: <28/01/01 21:23:58 keuchel@lightning>

#include <windows.h>
#include <winsock.h>
#include "celib.h"

struct servent *
xcegetservbyname(const char *sname, const char *sproto)
{
  static struct servent serv;
  static char name[64];
  static char proto[64];

  FILE *f;
  char path[MAX_PATH];
  int port;
  char buf[512];

  strcpy(path, XCEGetUnixPath("/etc/services"));

  if((f = xcefopen(path, "r")) == NULL)
    {
      XCEShowMessageA("Cannot open %s", path);
      return NULL;
    }

  while(fgets(buf, sizeof(buf), f))
    {
      if(buf[0] == '#' || buf[0] == '\n')
	continue;

      if(sscanf(buf, "%s %d/%s", name, &port, proto) == 3)
	{
	  if(strcmp(sname, name) == 0 && strcmp(sproto, proto) == 0)
	    {
	      serv.s_name = name;
	      serv.s_aliases = NULL;
	      serv.s_port = htons((unsigned short) port);
	      serv.s_proto = proto;

	      xcefclose(f);
	      return &serv;
	    }
	}
    }

  XCEShowMessageA("Cannot find service %s", sname);
  errno = ENOENT;

  return NULL;
}
