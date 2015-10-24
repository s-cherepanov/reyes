#ifndef parameter_h
#define parameter_h
/*------------------------------------------------------
 * TITLE:  parameter.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Parameter definitions
 * (C)     Copyright $Date: 1997/11/16 14:21:49 $
 * $Log: parameter.h,v $
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:16:13  ady
 * Added strint parameter
 *
 * Revision 1.1  1997/03/10 21:58:23  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdlib.h>

// C++ includes
#include "iostream.h"

// Reyes includes
#include "colour.h"

#define PARAMTYPE_NONE    0
#define PARAMTYPE_INT     1
#define PARAMTYPE_FLOAT   2
#define PARAMTYPE_STRING  3
#define PARAMTYPE_VECTOR  4

class Parameter
{
  public:
    Parameter(){type=PARAMTYPE_NONE;}
    Parameter(int _i){type=PARAMTYPE_INT; data.i=_i;}
    Parameter(float _f){type=PARAMTYPE_FLOAT; data.f=_f;}
    Parameter(double _d){type=PARAMTYPE_FLOAT; data.f=_d;}
    Parameter(Vector3 v){type=PARAMTYPE_VECTOR; data.vector=new Vector3(v);}
    Parameter(char *_string){type=PARAMTYPE_STRING; data.string=(char*)malloc(strlen(_string)+1);
                             strcpy(data.string,_string);}

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Parameter &p);

    // Access to values
    int Int()
      {return (type==PARAMTYPE_INT)?data.i:0;}
    float Float()
      {return (type==PARAMTYPE_FLOAT)?data.f:((type==PARAMTYPE_INT)?data.i:0);}
    char* String()
      {return (type==PARAMTYPE_STRING)?data.string:(char*)NULL;}
    Vector3 Vector()
      {return (type==PARAMTYPE_VECTOR)?*data.vector:Vector3(0,0,0);}

  public:
    int type;
    union
    {
      int i;
      float f;
      char *string;
      Vector3 *vector;
    }data;
};

#endif
