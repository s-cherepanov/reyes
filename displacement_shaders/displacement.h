#ifndef displacement_h
#define displacement_h
/*------------------------------------------------------
 * TITLE:  displacement.h
 * ENVIR:  RiscBSD
 * DESC:   Displacement shader class
 * (C)     Copyright $Date: 1997/11/29 18:00:50 $
 * $Log: displacement.h,v $
 * Revision 1.1  1997/11/29 18:00:50  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "paramlist.h"
#include "point3.h"

class Displacement
{
  public:
    virtual float Value(Point3 P,float s,float t,Vector3 N)
      {return 0;}

    virtual void Dump()
      {cerr << "No such displacement\n";}
};

#endif
