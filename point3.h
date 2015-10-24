#ifndef point3_h
#define point3_h
/*------------------------------------------------------
 * TITLE:  point3.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   3D point class
 * (C)     Copyright $Date: 1997/11/16 14:21:49 $
 * $Log: point3.h,v $
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:16:13  ady
 * Minor cosmetic changes
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include "iostream.h"
#include "iomanip.h"

// Reyes includes
#include "vector3.h"

class Point3: public Vector3
{
  public:
    Point3(float _x=0,float _y=0,float _z=0):Vector3(_x,_y,_z){} // Constructor

    Point3(Vector3 v){x=v.x; y=v.y; z=v.z;}
};

#endif
