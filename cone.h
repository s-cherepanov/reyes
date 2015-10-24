#ifndef cone_h
#define cone_h
/*------------------------------------------------------
 * TITLE:  cone.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Cone primitive class
 * (C)     Copyright $Date: 1998/12/10 20:14:17 $
 * $Log: cone.h,v $
 * Revision 1.3  1998/12/10 20:14:17  ady
 * Changed dicing
 *
 * Revision 1.2  1997/11/29 18:09:05  ady
 * Uses new lights and new dicing method
 *
 * Revision 1.1  1997/11/23 15:41:47  ady
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

class Cone: public Primitive
{
  public:
    Cone(float height=1,float radius=1,float thetamin=0,float thetamax=360,
         float zmin=0,float zmax=1); // Constructor

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Cone &s);
    void Dump();

    bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector);

    void DoDice(MicroGrid &microgrid,int us,int vs);

    bool Splitable();
    void Split(list<Primitive*> &primlist);

    bool EyeBound(BoundBox3 &bb);

  private:
    float height;
    float radius;
    float thetamin,thetamax;
    float zmin,zmax;
};

#endif
