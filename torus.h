#ifndef torus_h
#define torus_h
/*------------------------------------------------------
 * TITLE:  torus.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Torus primitive class
 * (C)     Copyright $Date: 1998/12/10 20:18:44 $
 * $Log: torus.h,v $
 * Revision 1.2  1998/12/10 20:18:44  ady
 * scaling now in Ri structure
 *
 * Revision 1.1  1997/11/29 18:12:35  ady
 * Initial revision
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

class Torus: public Primitive
{
  public:
    Torus(float major_radius=1,float minor_radius=1,float phimin=0,float phimax=360,
          float arcmin=0,float arcmax=360); // Constructor

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Torus &s);
    void Dump();

    bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector);

    void DoDice(MicroGrid &microgrid,int us,int vs);

    bool Splitable();
    void Split(list<Primitive*> &primlist);

    bool EyeBound(BoundBox3 &bb);

  private:
    float major_radius;
    float minor_radius;
    float phimin,phimax;
    float arcmin,arcmax;
};

#endif
