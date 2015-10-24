#ifndef patch_h
#define patch_h
/*------------------------------------------------------
 * TITLE:  patch.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Bicubic patch primitive class
 * (C)     Copyright $Date: 1997/11/29 18:10:42 $
 * $Log: patch.h,v $
 * Revision 1.2  1997/11/29 18:10:42  ady
 * Now uses stdc list class
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.1  1997/08/28 00:16:13  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes

// Reyes includes
#include "primitive.h"
#include "point3.h"

class Patch: public Primitive
{
  public:
    Patch(Point3 control[4][4]);
    Patch(Point3 c00,Point3 c01,Point3 c02,Point3 c03,
          Point3 c10,Point3 c11,Point3 c12,Point3 c13,
          Point3 c20,Point3 c21,Point3 c22,Point3 c23,
          Point3 c30,Point3 c31,Point3 c32,Point3 c33);

    BoundBox3 GetBounds();

    void Dump();

    bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector);

    bool Diceable(float vpscale);
    void Dice(MicroGrid &microgrid,float vpscale);

    bool Splitable();
    void Split(list<Primitive*> &primlist);

    bool EyeBound(BoundBox3 &bb);

    void EstimateGridSize(float vpscale,int &us,int &vs);

    void SetTextureCoords(float _smin,float _tmin,float _smax,float _tmax)
      {smin=_smin; tmin=_tmin; smax=_smax; tmax=_tmax;}

    friend ostream &operator<<(ostream &io,const Patch &p);

  private:
    Point3 control[4][4];
    float smin,tmin,smax,tmax;
};

#endif
