#ifndef boundbox3_h
#define boundbox3_h
/*------------------------------------------------------
 * TITLE:  boundbox3.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Bounding box utilities
 * (C)     Copyright $Date: 1998/12/10 20:13:10 $
 * $Log: boundbox3.h,v $
 * Revision 1.3  1998/12/10 20:13:10  ady
 * Added EyeToScreenBound
 *
 * Revision 1.2  1997/11/23 15:41:47  ady
 * Added transformation operator
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:16:13  ady
 * Added EyeToScreenBound()
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "point3.h"
#include "matrix4.h"

class BoundBox3
{
  public:
    BoundBox3(Point3 min=Point3(0,0,0),Point3 max=Point3(1,1,1));

    friend BoundBox3 operator*(Matrix4 transform,BoundBox3 box);

    // Stream input/output
    friend ostream &operator<<(ostream &io,const BoundBox3 &bbox);

    void SortMinMax();              // Sort x,y,z min and max so min<max.

    Point3 min,max;
};

#endif
