/*------------------------------------------------------
 * TITLE:  paramlist.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Paramlist definitions
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: paramlist.cpp,v $
 * Revision 1.4  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.3  1998/12/10 20:09:08  ady
 * Added int
 *
 * Revision 1.2  1997/11/29 18:02:41  ady
 * Now uses new stdc list class
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "paramlist.h"
#include "list.h"
#include "iterator.h"
#include "bool.h"

list<Parameter>::iterator Find(list<Parameter> &parameters,char *name,bool &found)
{
  int i=0;
  Parameter param;
  list<Parameter>::iterator iter,iter_found;

  found=FALSE;
  iter=parameters.begin();
  for(i=0;i<parameters.size();i++)
  {
    param=*iter;
    if ((param.type==PARAMTYPE_STRING)&&
        (!strcmp(param.data.string,name)))
    {
      found=TRUE;
      iter_found=iter;
    }
    iter++;
  }
  return iter_found;
}

bool ParamList::Exists(char *name)
{
  bool found;
  Find(parameters,name,found);
  return found;
}

float ParamList::Float(char *name)
{
  list<Parameter>::iterator iter;
  bool found;

  iter=Find(parameters,name,found);
  if ((found)&&(iter!=parameters.end()))
  {
    iter++;
    return (*iter).Float();
  }
  else
    return 0;
}

int ParamList::Int(char *name)
{
  list<Parameter>::iterator iter;
  bool found;

  iter=Find(parameters,name,found);
  if ((found)&&(iter!=parameters.end()))
  {
    iter++;
    return (*iter).Int();
  }
  else
    return 0;
}

char* ParamList::String(char *name)
{
  list<Parameter>::iterator iter;
  bool found;

  iter=Find(parameters,name,found);
  if ((found)&&(iter!=parameters.end()))
  {
    iter++;
    return (*iter).String();
  }
  else
    return 0;
}

Vector3 ParamList::Vector(char *name)
{
  list<Parameter>::iterator iter;
  bool found;

  iter=Find(parameters,name,found);
  if ((found)&&(iter!=parameters.end()))
  {
    iter++;
    return (*iter).Vector();
  }
  else
    return 0;
}

