/*
# proc: extractmit - returns the string containing an mis filename and entry
# proc:              index from the specified position of an mit structure.
*/

#include <stdio.h>
#include <string.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

char *extractmit(mitptr, num)
MIT *mitptr;
int num;
{
  char *value;

  if (num>mitptr->num-1)
     fatalerr("extractmit","Invalid request entry number",NULL);
  if ((value = strdup(mitptr->values[num]))== NULL)
     syserr("extractmit","strdup",mitptr->values[num]);
  return(value);
}
