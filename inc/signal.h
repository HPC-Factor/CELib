#ifndef SIGNAL_H
#define SIGNAL_H 1

/* Signal types from vc60. The numbers correspond to sys_siglist in 
   emacs process.c.
 */
#define SIGINT          2 /* interrupt */
#define SIGILL          4 /* illegal instruction - invalid function image */
#define SIGFPE          8 /* floating point exception */
#define SIGSEGV         11 /* segment violation */
#define SIGTERM         15 /* Software termination signal from kill */
#define SIGBREAK        21 /* Ctrl-Break sequence */
#define SIGABRT         22 /* abnormal termination triggered by abort call */

#define NSIG            22

/* signal action codes */

#define SIG_DFL (void (*)(int))0           /* default signal action */
#define SIG_IGN (void (*)(int))1           /* ignore signal */
#define SIG_SGE (void (*)(int))3           /* signal gets error */
#define SIG_ACK (void (*)(int))4           /* acknowledge */
#define SIG_ERR (void (*)(int))-1          /* signal error value */

#endif
