#ifndef paramlist_h
#define paramlist_h
/*------------------------------------------------------
 * TITLE:  paramlist.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Paramlist definitions
 * (C)     Copyright $Date: 1997/11/29 18:10:42 $
 * $Log: paramlist.h,v $
 * Revision 1.2  1997/11/29 18:10:42  ady
 * Now uses stdc list class
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "list.h"
#include "bool.h"
#include "parameter.h"
#include "iterator.h"

class ParamList
{
  public:
    ParamList(){}
    bool Exists(char *name);

    int Int(char *name);
    float Float(char *name);
    char* String(char *name);
    Vector3 Vector(char *name);
    int Length(){return parameters.size();}
    void AddParameter(Parameter param){parameters.push_back(param);}
    
    list<Parameter> parameters;
};

#endif
