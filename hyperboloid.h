#ifndef hyperboloid_h
#define hyperboloid_h
/*------------------------------------------------------
 * TITLE:  hyperboloid.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Hyperboloid primitive class
 * (C)     Copyright $Date: 1998/12/10 20:15:39 $
 * $Log: hyperboloid.h,v $
 * Revision 1.2  1998/12/10 20:15:39  ady
 * Added tmin,tmax to hyperboloid
 *
 * Revision 1.1  1997/11/29 18:09:05  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include "iostream.h"
#include "iomanip.h"

// Misc includes
#include "list.h"

// Reyes includes
#include "primitive.h"
#include "point3.h"
#include "boundbox3.h"
#include "microgrid.h"
#include "camera.h"
#include "matrix4.h"

class Hyperboloid: public Primitive
{
  public:
    Hyperboloid(Point3 point1,Point3 point2,float thetamin,float thetamax,float tmin=0,float tmax=1); // Constructor

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Hyperboloid &s);
    void Dump();

    bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector);

    void DoDice(MicroGrid &microgrid,int us,int vs);

    bool Splitable();
    void Split(list<Primitive*> &primlist);

    bool EyeBound(BoundBox3 &bb);

  private:
    Point3 point1,point2;
    float thetamin,thetamax;
    float tmin,tmax;
};

#endif
