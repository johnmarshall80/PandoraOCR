/*
# proc: ramp_int_runs - takes a list of integers, thresholds them into
# proc:                 binary values and assigns incrementally accumulated
# proc:                 run length vales into a new list.
*/

#include <stdio.h>

ramp_int_runs(bins, rlist, blen, thresh)
int *bins, **rlist;
int blen, thresh;
{
   int i, count;

   (*rlist) = (int *)calloc(blen, sizeof(int));
   if((*rlist) == NULL)
      syserr("ramp_int_runs", "calloc", "rlist");

   count = 0;
   for(i = 0; i < blen; i++){
      if(bins[i] > thresh)
         (*rlist)[i] = ++count;
      else
         count = 0;
   }
}
