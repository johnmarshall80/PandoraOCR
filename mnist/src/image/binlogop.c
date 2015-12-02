/*
# proc: nandbinimage - logically NANDs two binary bitmaps together, returning
# proc:                the results in a new image.
*/

#include <stdio.h>

/***************************************************************/
nandbinimage(xdata, rdata, fdata, iw, ih)
unsigned char **xdata, *rdata, *fdata;
int iw, ih;
{
   int size, i;
   unsigned char *rptr, *fptr,  *xptr;

   size = SizeFromDepth(iw, ih, 1);
   (*xdata) = (unsigned char *)calloc(size, 1);
   if((*xdata) == NULL)
      syserr("nandbinimage", "calloc", "xdata");
   rptr = rdata;
   fptr = fdata;
   xptr = (*xdata);
   for(i = 0; i < size; i++){
      *xptr++ = *fptr++ & ~ *rptr++;
   }
}
