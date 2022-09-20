// uid.c
// 
// Time-stamp: <31/01/01 21:22:25 keuchel@lightning>

#include "celib.h"
#include "pwd.h"

int
setuid(uid_t id)
{
  return -1;
}

int
seteuid(uid_t id)
{
  return -1;
}

int
getuid()
{
  struct passwd *pwentry;
  char buf[256];
  char *p;

  if((p = xcegetenv("USERNAME")) == NULL)
    return -1;
  strcpy(buf, p);
  if((pwentry = getpwnam(buf)) == NULL)
    return -1;
  else
    return pwentry->pw_uid;
}

int
geteuid()
{
  return 0;
}

