// logwtmp.c
//
// Time-stamp: <31/01/01 21:20:11 keuchel@lightning>

#include <stdio.h>
#include <time.h>
#include "celib.h"

int
logwtmp(const char *ttyline, const char *user, const char *remotehost)
{
  FILE *f;
  time_t ti;

  ti = time(NULL);

  if((f = xcefopen(XCEGetUnixPath("/var/log/wtmp.log"), "a")) == NULL)
    return -1;

  if(strcmp(user, "") == 0 && strcmp(remotehost, "") == 0)
    fprintf(f, "%.24s: logout on line %s\n", ctime(&ti), ttyline);
  else
    fprintf(f, "%.24s: login from %s@%s on line %s\n",
	    ctime(&ti), 
	    (user) ? user : "?", 
	    (remotehost) ? remotehost : "?", 
	    (ttyline) ? ttyline : "?");
  xcefclose(f);
  return 0;
}
