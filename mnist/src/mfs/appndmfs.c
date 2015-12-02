/*
# proc: appendmfs - appends a specified string value to the end of an
# proc:             mfs structure.
*/

#include <stdio.h>
#include <string.h>
#include <mfs.h>

void appendmfs(mfs,value)
MFS *mfs;
char *value;
{
   char error[106];

   if(mfs->num >= mfs->alloc){
      sprintf(error,"attempting to update %d of %d",mfs->num, mfs->alloc);
      fatalerr("appendmfs","MFS memory OVERFLOW", error);
   }
   mfs->values[mfs->num] = strdup(value);
   if(mfs->values[mfs->num] == NULL)
      syserr("appendmfs","strdup", "mfs->values[]");
   (mfs->num)++;
}
