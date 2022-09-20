#ifndef UNISTD_H
#define UNISTD_H 1

#ifdef __cplusplus
extern "C" {
#endif

int getopt (int, char * const [], const char *);

extern	 char *optarg;
extern	 int opterr;
extern	 int optind;
extern	 int optopt;

#ifdef __cplusplus
};
#endif

#endif

