/*
# proc: get_mis_entw - returns an mis strutcure's entry width attribute.
# proc:
# proc: set_mis_entw - sets an mis structure's entry width attribute.
# proc:
# proc: get_mis_enth - returns an mis structure's entry height attribute.
# proc:
# proc: set_mis_enth - sets an mis structure's entry height attribute.
# proc:
*/

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <ihead.h>
#include<defs.h>

int get_mis_entw(head)
IHEAD *head;
{
  int mis_entw;

  if (head==NULL)
     fatalerr("get_mis_entw","pointer head is NULL",NULL);
  sscanf(head->par_x,"%d", &mis_entw);
  return(mis_entw);
}

set_mis_entw(head, mis_entw)
IHEAD *head;
int mis_entw;
{
  if (head==NULL)
     fatalerr("set_mis_entw","pointer head is NULL",NULL);
  memset(head->par_x,NULL,SHORT_CHARS);
  sprintf(head->par_x,"%d",mis_entw);
  return(True);
}

int get_mis_enth(head)
IHEAD *head;
{
  int mis_enth;

  if (head==NULL)
     fatalerr("get_mis_enth","pointer head is NULL",NULL);
  sscanf(head->par_y,"%d", &mis_enth);
  return(mis_enth);
}

set_mis_enth(head, mis_enth)
IHEAD *head;
int mis_enth;
{
  if (head==NULL)
     fatalerr("set_mis_enth","pointer head is NULL",NULL);
  memset(head->par_y,NULL,SHORT_CHARS);
  sprintf(head->par_y,"%d",mis_enth);
  return(True);
}
