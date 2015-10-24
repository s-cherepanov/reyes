#ifndef polygon_h
#define polygon_h
/*------------------------------------------------------
 * TITLE:  polygon.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Polygon primitive class
 * (C)     Copyright $Date: 1997/11/29 18:10:42 $
 * $Log: polygon.h,v $
 * Revision 1.2  1997/11/29 18:10:42  ady
 * Now uses stdc list class
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:16:13  ady
 * Modified TransformToEyeSpace() and SetTextureCoords()
 *
 * Revision 1.2  1997/03/10 21:58:23  ady
 * New structure for dicing and splitting
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Misc includes
#include "list.h"

// Reyes includes
#include "primitive.h"
#include "camera.h"
#include "microgrid.h"
#include "point3.h"
#include "matrix4.h"
#include "boundbox3.h"

class Polygon: public Primitive
{
  public:
    Polygon(Point3 p1,Point3 p2,Point3 p3,Point3 p4);

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Polygon &p);
    void Dump();

    bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector);

    bool Diceable(float vpscale);
    void Dice(MicroGrid &microgrid,float vpscale);

    bool Splitable();
    void Split(list<Primitive*> &primlist);

    void SetTextureCoords(float _smin,float _tmin,float _smax,float _tmax)
      {smin=_smin; tmin=_tmin; smax=_smax; tmax=_tmax;}

    bool EyeBound(BoundBox3 &bb);

  private:
    Point3 p[4];
    float smin,tmin,smax,tmax;
};

#endif
