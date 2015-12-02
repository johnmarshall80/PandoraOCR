/*
# proc: allocmfs - allocates and initiallizes an mfs structure of a
# proc:            specified length.
*/

#include <stdio.h>
#include <mfs.h>

MFS *allocmfs(mfslen)
int mfslen;
{
   MFS *mfs;
   int item, size;

   if((mfs = (MFS *)calloc(1, sizeof(MFS))) == NULL)
      syserr("allocmfs","malloc","mfs");
   mfs->alloc = mfslen;
   mfs->num = 0;
   size = mfs->alloc * sizeof(char *);
   if((mfs->values = (char **)calloc(mfs->alloc, sizeof(char *))) == NULL)
      syserr("allocmfs","calloc","mfs->values");
   return(mfs);
}
