/*------------------------------------------------------
 * TITLE:  parameter.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Parameter definitions
 * (C)     Copyright $Date: 1997/11/16 14:18:48 $
 * $Log: parameter.cpp,v $
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/10 22:00:39  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "parameter.h"

// C++ includes
#include "iostream.h"

ostream &operator<<(ostream &io,const Parameter &p)
{
  switch(p.type)
  {
    case PARAMTYPE_INT:
      io << p.data.i;
      break;
    case PARAMTYPE_FLOAT:
      io << p.data.f;
      break;
    case PARAMTYPE_STRING:
      io << p.data.string;
      break;
    case PARAMTYPE_VECTOR:
      io << *p.data.vector;
      break;
  }
  return io;
}
