/*
# proc: syserr - exits on error with a status of -1, printing to stderr a
# proc:          caller-defined message and the system call error associated
# proc:          with errno.
*/

#include <stdio.h>

void syserr(funcname, syscall, msg)
char *funcname, *syscall, *msg;
{
/*
//   extern int errno, sys_nerr;
//   extern char *sys_errlist[];
//   int err;
//
//   err = errno;
//
//   (void) fflush(stdout);
//   if(msg == NULL)
//      (void) fprintf(stderr,"ERROR: %s: %s (%d",funcname,syscall,err);
//   else
//      (void) fprintf(stderr,"ERROR: %s: %s: %s (%d",funcname,syscall,msg,err);
//   if(err > 0 && err < sys_nerr)
//      (void) fprintf(stderr,"; %s)\n",sys_errlist[err]);
//   else
//      (void) fprintf(stderr,")\n");
//   (void) fflush(stderr);
*/
   (void) fflush(stdout);
   if(msg == NULL)
      (void) fprintf(stderr,"ERROR: %s: %s ",funcname,syscall);
   else
      (void) fprintf(stderr,"ERROR: %s: %s: %s ",funcname,syscall,msg);
   (void) fflush(stderr);

   exit(-1);
}
