/*
# proc: writemitfile - writes the contents of a mit structure to the specified
# proc:                filename.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

void writemitfile(file,mitptr)
char *file;
MIT *mitptr;
{
  FILE *mit = NULL;
  int item;

  if ((mit = fopen(file,"w")) == NULL)
     syserr("writemitfile","fopen",file);
  fprintf(mit,"%d\n",mitptr->num);
  for (item=0;item < mitptr->num;item++)
      fprintf(mit,"%s\n",mitptr->values[item]);
  fclose(mit);
}
