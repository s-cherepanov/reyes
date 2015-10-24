#ifndef sphere_h
#define sphere_h
/*------------------------------------------------------
 * TITLE:  sphere.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Sphere primitive class
 * (C)     Copyright $Date: 1998/12/10 20:18:23 $
 * $Log: sphere.h,v $
 * Revision 1.4  1998/12/10 20:18:23  ady
 * Added texture coords
 *
 * Revision 1.3  1997/11/29 18:12:35  ady
 * New dicing method used
 *
 * Revision 1.2  1997/11/21 20:26:40  ady
 * Can now use Zmin,Zmax and arc usefully
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:16:13  ady
 * Added Scale(), Translate() and Rotate()
 *
 * Revision 1.2  1997/03/10 21:58:23  ady
 * New structure for dicing and splitting
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
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

class Sphere: public Primitive
{
  public:
    Sphere(float radius=1,float zmin=-1,float zmax=1,float arcmin=0,float arcmax=360); // Constructor

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Sphere &s);
    void Dump();

    bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector);

    bool Splitable();
    void Split(list<Primitive*> &primlist);

    void DoDice(MicroGrid &microgrid,int us,int vs);

    bool EyeBound(BoundBox3 &bb);

  private:
    float radius;
    float zmin,zmax;
    float arcmin,arcmax;
};

#endif
