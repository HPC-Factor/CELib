#ifndef CESOCKET_H
#define CESOCKET_H 1

#include <winsock.h>

#ifdef __cplusplus
extern "C" {
#endif

int xcewsstart();
void xcewsstop();
void xcewserror(char *fmt, ...);

SOCKET xcercmd (char **ahost, unsigned short rport, const char *locuser, 
		const char *remuser, const char *cmd, int *fd2p);

SOCKET xcerexec(char **ahost, unsigned short rport, char *user, 
		char *pass, char *cmd, int *fd2p);

#ifdef __cplusplus
};
#endif

#endif
