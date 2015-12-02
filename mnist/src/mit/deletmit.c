#include <stdio.h>
#include <string.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

deletemit(mit, index)
MIT *mit;
int index;
{
  char *value;
  int i;

  if (index > mit->num - 1)
     fatalerr("deletemit","Invalid entry index",NULL);
  if(mit->num <= 0)
     fatalerr("deletemit","Value list is empty",NULL);
  free(mit->values[index]);
  for(i = index+1; i < mit->num; i++){
     mit->values[i-1] = mit->values[i];
  }
  mit->num--;
  mit->values[mit->num] = NULL;
}
