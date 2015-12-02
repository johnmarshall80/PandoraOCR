/*
# proc: readmfsfile - reads the contents of the specified file into a
# proc:               new mfs structure.
# proc: readmfsint2 -  reads MFS file containing 2 integers strings per value
# proc:                from the specified file and returns them in 2 separate
# proc:                lists of integers.
# proc: readmfshdr - reads the header of the specified file into an int.
*/

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <mfs.h>
 

/*******************************************************************/
MFS *readmfsfile(file)
char *file;
{
  MFS *mfsptr = NULL;
  FILE *mfs = NULL;
  int feature_number = 0;
  int ac;
  char line[MAXMFSLENGTH],**value;

  mfs = fopen(file,"r");
  if (mfs == NULL)
     syserr("readmfsfile","fopen",file);

  if (fgets(line,MAXMFSLENGTH-1,mfs) == (char *)NULL) 
     syserr("readmfsfile","fgets",NULL);
  if (sscanf(line,"%d",&ac) != 1)
     syserr("readmfsfile","sscanf","ac");
  if (ac <1)
     fatalerr("readmfsfile",file,"count less then 1");
  if ((value = (char **)malloc(sizeof(char *) * ac)) == NULL)
     syserr("readmfsfile","malloc","value");
  for (feature_number = 0;feature_number<ac; feature_number+=1)
      {
         if(fgets(line, sizeof(line),mfs) == NULL)
            fatalerr("readmfsfile",file,"EOF unexpected");
         line[strlen(line)-1] = '\0';
         value[feature_number] = strdup(line);
         if(value[feature_number] == NULL)
            syserr("readmfsfile","strdup","value[]");
      }
  fclose(mfs);
  if ((mfsptr = (MFS *)malloc(sizeof(MFS))) == NULL)
     syserr("readmfsfile","malloc","mfsptr");
  mfsptr->alloc = ac;
  mfsptr->num = ac;
  mfsptr->values = value;
  return(mfsptr);
}

 
/*******************************************************************/
int readmfshdr(file)
char *file;
{
  FILE *mfs = NULL; 
  int ac;
  char line[MAXMFSLENGTH],**value;
 
  if ((mfs = fopen(file,"r")) == NULL)
     syserr("readmfshdr","fopen",file);
  if (fgets(line,MAXMFSLENGTH-1,mfs) == (char *)NULL)
     syserr("readmfshdr","fgets",NULL);
  fclose(mfs); 
 
  if (sscanf(line,"%d",&ac) != 1)
     syserr("readmfshdr","sscanf","ac");
  if (ac <1)
     fatalerr("readmfshdr",file,"count less then 1");
  return(ac);
}

/*******************************************************************/
readmfsint2(pfile, xref, yref, nref)
char *pfile;
int **xref, **yref, *nref;
{
   MFS *mfs;

   mfs = readmfsfile(pfile);
   mfs2int2(mfs, xref, yref);
   *nref = mfs->num;
}
