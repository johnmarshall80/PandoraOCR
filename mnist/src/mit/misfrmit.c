/*
# proc: misfrmit - creates a new mis structure containing entries extracted
# proc:            from mis files at specified indides based on the contents
# proc:            of a mit structure.
*/

#include <stdio.h>
#include <sys/param.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

MIS *misfrmit(mit)
MIT *mit;
{
   MIS *new_mis, *misptr;
   IHEAD *first_head;
   FILE *fp;
   char **mis_names;
   char mis_file[MAXPATHLEN];
   char *value;
   unsigned char *image_data;
   int *index, *used;
   int i, j;
   int depth, bpi, entw, enth;

   if((mis_names = (char **)calloc(mit->num, sizeof(char *))) == NULL)
      syserr("misfrmit","calloc","mis_names");
   if((index = (int *)calloc(mit->num, sizeof(int))) == NULL)
      syserr("misfrmit","calloc","index");
   if((used = (int *)calloc(mit->num, sizeof(int))) == NULL)
      syserr("misfrmit","calloc","used");

   for(i = 0; i < mit->num; i++) {
      if((mis_names[i] = (char *)calloc(MAXPATHLEN, sizeof(char))) == NULL)
         syserr("misfrmit","calloc","mis_names[i]");
      value = extractmit(mit, i);
      if(sscanf(value, "%s %d", mis_names[i], &(index[i])) != 2)
         syserr("misfrmit","sscanf",value);
      free(value);      
   }

   if((fp = fopen(mis_names[0],"r")) == NULL)
      syserr("misfrmit","fopen",mis_names[0]);
   first_head = readihdr(fp);
   fclose(fp);
   depth = get_depth(first_head);
   bpi = get_density(first_head);
   entw = get_mis_entw(first_head);
   enth = get_mis_enth(first_head);
   free(first_head);
   new_mis = allocmis(NULL,depth,bpi,entw,enth,mit->num);
   new_mis->ent_num = new_mis->ent_alloc;

   for(i = 0; i < mit->num; i++) {
      if(used[i] == 0) {
         misptr = readmisfile(mis_names[i]);
         for(j = i; j < mit->num; j++) {
            if(strcmp(mis_names[i], mis_names[j]) == 0) {
               used[j] = 1;
               image_data = extractmis(misptr, index[j]);
               changemis(new_mis, j, image_data);
               free(image_data);
	    }
         }
         freemis(misptr);
      }
      free(mis_names[i]);
   }
   free(used);
   free(index);
   free(mis_names);
   return(new_mis);
}
