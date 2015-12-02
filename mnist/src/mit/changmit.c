/*
# proc: changemit - replaces the value at a specified position with an mit
# proc:             structure with a new mis filename and entry index.
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

changemit(mit, index, mis_file, mis_index)
MIT *mit;
int index, mis_index;
char *mis_file;
{
  int i;
  char *value=NULL;

  if (index > mit->num - 1)
     fatalerr("changemit","Invalid entry index",NULL);
  if(mit->num <= 0)
     fatalerr("changemit","Value list is empty",NULL);
  free(mit->values[index]);
  if((value = malloc(MAXMITLENGTH)) == NULL)
     syserr("appendmit","malloc","value");
  sprintf(value, "%s  %d", mis_file, index);
  if((mit->values[index] = strdup(value)) == NULL)
     syserr("changemit", "strdup", "value");
}
