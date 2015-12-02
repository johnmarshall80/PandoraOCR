/*
# proc: shuffle_index - generates a randomly ordered list of indices up to a
# proc:                 specified length.
# proc: shuffle_strings - takes an array of strings and returns the list
# proc:                   randomly shuffled.
# proc: shuffle_integers - takes an array of integers and returns the list
# proc:                    randomly shuffled.

*/

#include <stdio.h>
#include <string.h>
#include <shuffle.h>

int *shuffle_index(num_shuffles, num_index)
int num_shuffles, num_index;
{
   int *array;
   INDEX *istruct;
   int shuffle, i;

   array = (int *) malloc(num_index * sizeof(int));
   if(array == NULL)
      syserr("shuffle_index","malloc","array");
   with_1_INDEX(istruct, MAX_INDEX, INTTYPE)
      for(i = 0; i < num_index; i++)
         add_to_1_INDEX(istruct, i, random());
      multisort_1_INDEX(istruct, INC);
      for(shuffle = 1; shuffle < num_shuffles; shuffle++) {
         foreach_in_INDEX(istruct)
            this_index(0) = random();
         endfor
         multisort_1_INDEX(istruct, INC);
      }
      i = 0;
      foreach_in_INDEX(istruct)
         array[i++] = this_item_int;
      endfor
   end_with_INDEX
   return(array);
}


char **shuffle_strings(num_shuffles, strlist, strlen)
char **strlist;
int num_shuffles, strlen;
{
   char **array;
   INDEX *istruct;
   int shuffle, i;

   with_1_INDEX(istruct, MAX_INDEX, PTRTYPE)
      for(i = 0; i < strlen; i++){
         add_to_1_INDEX(istruct, strlist[i], random());
      }
      multisort_1_INDEX(istruct, INC);
      for(shuffle = 1; shuffle < num_shuffles; shuffle++) {
         foreach_in_INDEX(istruct)
            this_index(0) = random();
         endfor
         multisort_1_INDEX(istruct, INC);
      }
      array = (char **) malloc(strlen * sizeof(char *));
      if(array == NULL)
         syserr("shuffle_strings","malloc","array");
      i = 0;
      foreach_in_INDEX(istruct)
//         if((array[i++] = strdup(this_item_ptr)) == NULL)
//            syserr("shuffle_strings", "strdup", "array[i]");
      endfor
   end_with_INDEX
   return(array);
}


int *shuffle_integers(num_shuffles, intlist, intlen)
int *intlist;
int num_shuffles, intlen;
{
   int *array;
   INDEX *istruct;
   int shuffle, i;

   with_1_INDEX(istruct, MAX_INDEX, INTTYPE)
      for(i = 0; i < intlen; i++){
         add_to_1_INDEX(istruct, intlist[i], random());
      }
      multisort_1_INDEX(istruct, INC);
      for(shuffle = 1; shuffle < num_shuffles; shuffle++) {
         foreach_in_INDEX(istruct)
            this_index(0) = random();
         endfor
         multisort_1_INDEX(istruct, INC);
      }
      array = (int *) malloc(intlen * sizeof(int));
      if(array == NULL)
         syserr("shuffle_integers","malloc","array");
      i = 0;
      foreach_in_INDEX(istruct)
         array[i++] = this_item_int;
      endfor
   end_with_INDEX
   return(array);
}

