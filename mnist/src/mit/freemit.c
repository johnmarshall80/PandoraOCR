/*
# proc: freemit - deallocates the memory associated with the specified
# proc:           mit structure.
*/

#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

void freemit(mitptr)
MIT *mitptr;
{
  int item;
  for (item=0;item<mitptr->num;item++)
      free(mitptr->values[item]);
  free((char *)mitptr->values);
  free((char *)mitptr);
}
