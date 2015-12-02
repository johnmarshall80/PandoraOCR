/*
# proc: readmitfile - reads the contents of the specified file into a new
# proc:               mit structure.
*/

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <ihead.h>
#include <mis.h>
#include <mfs.h>
#include <mit.h>

MIT *readmitfile(file)
char *file;
{
  MIT *mitptr = NULL;
  FILE *mit = NULL;
  int feature_number = 0;
  int ac;
  char line[MAXMITLENGTH],**value;

  mit = fopen(file,"r");
  if (mit == NULL)
     syserr("readmitfile","fopen",file);

  if (fgets(line,MAXMITLENGTH-1,mit) == (char *)NULL) 
     syserr("readmitfile","fgets",NULL);
  if (sscanf(line,"%d",&ac) != 1)
     syserr("readmitfile","sscanf","ac");
  if (ac <1)
     fatalerr("readmitfile",file,"count less then 1");
  if ((value = (char **)malloc(sizeof(char *) * ac)) == NULL)
     syserr("readmitfile","malloc","value");
  for (feature_number = 0;feature_number<ac; feature_number+=1)
      {
         if(fgets(line, sizeof(line),mit) == NULL)
            fatalerr("readmitfile",file,"EOF unexpected");
         line[strlen(line)-1] = '\0';
         value[feature_number] = strdup(line);
         if(value[feature_number] == NULL)
            syserr("readmisfile","strdup","value[]");
      }
  fclose(mit);
  if ((mitptr = (MIT *)malloc(sizeof(MIT))) == NULL)
     syserr("readmitfile","malloc","mitptr");
  mitptr->alloc = ac;
  mitptr->num = ac;
  mitptr->values = value;
  return(mitptr);
}
