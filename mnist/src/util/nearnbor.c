/*
# proc: nearest_nbor - returns the location of the point from a list of points
# proc:                that is closest to another specified point and returns
# proc:                the closest distance.
*/

#include <values.h>

/***********************************************************************/
/* nearest_nbor nearest neighbor routine */
/* Darrin L. Dimmick 5-21-93 */
/* Returns the point from esnx and esny which is closest to point_x, point_y */
/* in nearest_x, nearest_y */
/* esnc is the number of values in ensx. It is also the number of values */
/* in esny */  

int nearest_nbor(nindex, ndistance, point_x, point_y, esnx, esny, esnc)
int *nindex, *ndistance, point_x, point_y, *esnx, *esny, esnc;
{
  int i, success, ni, nd, dist;

  ni = -1;
  nd = MAXINT;
  for(i = 0; i < esnc; i++){
// ATTN TODO     dist = diff_sq_distance(point_x, point_y, esnx[i], esny[i]);
     if (dist < nd){
         ni = i;
         nd = dist;
     } /* if */
   } /* for */
  *nindex = ni;
  *ndistance  = nd;
  if (ni < 0)
     success = 0;
  else
     success = 1;
  return(success);
}
