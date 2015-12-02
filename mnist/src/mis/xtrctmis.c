/*
# proc: extractmis - returns the mis entry from the specified position within
# proc:              an mis structure.
*/

#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <ihead.h>
#include <mis.h>

unsigned char *extractmis(mis,n)
MIS *mis;
int n;
{
   unsigned char *rbuf;
   char index[BUFSIZ];
   int depth, entlen, mislen;

   if ((n < 0) || (n >= mis->ent_num)){
        sprintf(index,"%d",n);
	fatalerr("extractmis","bad index",index);
   }

   if (mis->misw != mis->entw){
	sprintf(index,"mis width %d != image width %d", mis->misw, mis->entw);
	fatalerr("extractmis",index,(char *)NULL);
   }

   if (mis->mish != mis->ent_alloc * mis->enth)
	fatalerr("extractmis",
              "mis height != mis entries*image height",(char *)NULL);
   depth = get_depth(mis->head);
   entlen = SizeFromDepth(mis->entw,mis->enth,depth);
   mislen = SizeFromDepth(mis->misw,n*mis->enth,depth);
   if (entlen < 1)
      fatalerr("extractmis","size < 1",NULL);
   rbuf = (u_char *) malloc((u_int)entlen);
   if (rbuf == (u_char *) NULL)
	syserr("extractmis","malloc","rbuf");
   memcpy(rbuf,mis->data+mislen,entlen);
   return(rbuf);
}
