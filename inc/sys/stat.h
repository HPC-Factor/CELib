#ifndef SYS_STAT_H
#define SYS_STAT_H 1

#include <celib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct xcestat 
{
  dev_t st_dev;
  ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  dev_t st_rdev;
  off_t st_size;
  time_t st_atime;
  time_t st_mtime;
  time_t st_ctime;
};

int xcestat(const char *f, struct xcestat *st);

#ifdef __cplusplus
};
#endif

#endif
