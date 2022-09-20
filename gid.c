#include "pwd.h"

int
setgid(int id)
{
  return -1;
}

int
setegid(int id)
{
  return -1;
}

int
getgid()
{
  return 0;
}

int
getegid()
{
  return 0;
}

int 
initgroups(const char *groupname, gid_t id)
{
  return -1;
}
