/*
# proc: allocmit - allocates and initializes a new mit structure of specified
# proc:            length.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

MIT *allocmit(mitlen)
int mitlen;
{
   MIT *mit;
   int item, size;

   if((mit = (MIT *)malloc(sizeof(MIT))) == NULL)
      syserr("allocmit","malloc","mit");
   mit->alloc = mitlen;
   mit->num = 0;
   size = mit->alloc * sizeof(char *);
   if((mit->values = (char **)malloc(size)) == NULL)
      syserr("allocmit","malloc","mit->values");
   bzero(mit->values,size);
   return(mit);
}
