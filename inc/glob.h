/*	$NetBSD: glob.h,v 1.11 1998/06/22 12:01:43 tv Exp $	*/

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)glob.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _GLOB_H_
#define	_GLOB_H_

#include <sys/cdefs.h>
#ifndef UNDER_CE
#include <sys/featuretest.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

typedef struct {
	int gl_pathc;		/* Count of total paths so far. */
	int gl_matchc;		/* Count of paths matching pattern. */
	int gl_offs;		/* Reserved at beginning of gl_pathv. */
	int gl_flags;		/* Copy of flags parameter to glob. */
	char **gl_pathv;	/* List of paths matching pattern. */
				/* Copy of errfunc parameter to glob. */
	int (*gl_errfunc) __P((const char *, int));

	/*
	 * Alternate filesystem access methods for glob; replacement
	 * versions of closedir(3), readdir(3), opendir(3), stat(2)
	 * and lstat(2).
	 */
	void (*gl_closedir) __P((void *));
	struct dirent *(*gl_readdir) __P((void *));	
	void *(*gl_opendir) __P((const char *));

#ifdef UNDER_CE
	int (*gl_lstat) __P((const char *, struct xcestat *));
	int (*gl_stat) __P((const char *, struct xcestat *));
#else
#ifdef __LIBC12_SOURCE__
	int (*gl_lstat) __P((const char *, struct stat12 *));
	int (*gl_stat) __P((const char *, struct stat12 *));
#else
	int (*gl_lstat) __P((const char *, struct stat *));
	int (*gl_stat) __P((const char *, struct stat *));
#endif
#endif
} glob_t;

#define	GLOB_APPEND	0x0001	/* Append to output from previous call. */
#define	GLOB_DOOFFS	0x0002	/* Use gl_offs. */
#define	GLOB_ERR	0x0004	/* Return on error. */
#define	GLOB_MARK	0x0008	/* Append / to matching directories. */
#define	GLOB_NOCHECK	0x0010	/* Return pattern itself if nothing matches. */
#define	GLOB_NOSORT	0x0020	/* Don't sort. */
#define	GLOB_NOESCAPE	0x1000	/* Disable backslash escaping. */

#define	GLOB_NOSPACE	(-1)	/* Malloc call failed. */
#define	GLOB_ABORTED	(-2)	/* Unignored error. */
#define	GLOB_NOMATCH	(-3)	/* No match, and GLOB_NOCHECK was not set. */
#define	GLOB_NOSYS	(-4)	/* Implementation does not support function. */

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE)
#define	GLOB_ALTDIRFUNC	0x0040	/* Use alternately specified directory funcs. */
#define	GLOB_BRACE	0x0080	/* Expand braces ala csh. */
#define	GLOB_MAGCHAR	0x0100	/* Pattern had globbing characters. */
#define	GLOB_NOMAGIC	0x0200	/* GLOB_NOCHECK without magic chars (csh). */
#define	GLOB_TILDE	0x0800	/* Expand tilde names from the passwd file. */
#define	GLOB_QUOTE	0		/* source compatibility */

#define	GLOB_ABEND	GLOB_ABORTED	/* source compatibility */
#endif

__BEGIN_DECLS
#ifdef UNDER_CE
int	glob __P((const char *, int, int (*)(const char *, int), glob_t *));
void	globfree __P((glob_t *));
#else
#ifdef __LIBC12_SOURCE__
int	glob __P((const char *, int, int (*)(const char *, int), glob_t *));
void	globfree __P((glob_t *));
#else
int	glob __P((const char *, int, int (*)(const char *, int), glob_t *))
	__RENAME(__glob13);
void	globfree __P((glob_t *)) __RENAME(__globfree13);
#endif
#endif
__END_DECLS

#endif /* !_GLOB_H_ */
