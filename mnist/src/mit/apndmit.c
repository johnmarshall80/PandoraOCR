/*
# proc: appendmit - takes an mis filename and an mis entry index and appends
# proc:             them as a single string to the end of an mit structure.
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/param.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

void appendmit(mit,mis_file,index)
MIT *mit;
char *mis_file;
int index;
{
   char error[106];
   char *value=NULL;

   if(mit->num >= mit->alloc){
      sprintf(error,"attempting to update %d of %d",mit->num, mit->alloc);
      fatalerr("appendmit","MIT memory OVERFLOW", error);
   }
   if((value = malloc(MAXMITLENGTH)) == NULL)
      syserr("appendmit","malloc","value");
   sprintf(value, "%s  %d", mis_file, index);
   mit->values[mit->num] = strdup(value);
   if(mit->values[mit->num] == NULL)
      syserr("appendmit","strdup", "mit->values[]");
   (mit->num)++;
}
