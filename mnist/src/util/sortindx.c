/*
# proc: sort_index_dec_on_int - sorts an integer list into descending order.
# proc:
# proc: sort_index_on_int - sorts an integer list into ascending order.
# proc:
*/

#include <stdio.h>
#include <multsort.h>
#include <maxlist.h>

/*****************************************************************/
int *sort_index_dec_on_int(i1, num)
int *i1, num;
{
   int i, *s;

   s = (int *)calloc(num, sizeof(int));
   if(s == NULL)
      syserr("sort_index_on_int", "calloc", "s");
   with_1_INDEX(istruct, MAX_INDEX, INTTYPE)
      for(i = 0; i < num; i++)
         add_to_1_INDEX(istruct, i, i1[i]);
      multisort_1_INDEX(istruct, DEC);
      i = 0;
      foreach_in_INDEX(istruct)
         s[i++] = this_item_int;
      endfor
   end_with_INDEX
   return(s);
}

/*****************************************************************/
int *sort_index_on_int(i1, num)
int *i1, num;
{
   int i, *s;

   s = (int *)calloc(num, sizeof(int));
   if(s == NULL)
      syserr("sort_index_on_int", "calloc", "s");
   with_1_INDEX(istruct, MAX_INDEX, INTTYPE)
      for(i = 0; i < num; i++)
         add_to_1_INDEX(istruct, i, i1[i]);
      multisort_1_INDEX(istruct, INC);
      i = 0;
      foreach_in_INDEX(istruct)
         s[i++] = this_item_int;
      endfor
   end_with_INDEX
   return(s);
}
