#ifndef micropoly_h
#define micropoly_h
/*------------------------------------------------------
 * TITLE:  micropoly.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Micropolygon utilities
 * (C)     Copyright $Date: 1998/12/10 20:16:34 $
 * $Log: micropoly.h,v $
 * Revision 1.2  1998/12/10 20:16:34  ady
 * Added opacity
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:16:13  ady
 * Changed scalar_t's back to float's
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "point3.h"
#include "colour.h"
#include "framebuffer.h"
#include "matrix4.h"

class MicroPolygon
{
  public:
    MicroPolygon::MicroPolygon();
    MicroPolygon(Point3 &p1,Point3 &p2,Point3 &p3,Point3 &p4,Colour &colour,Opacity &opacity);

    friend ostream &operator<<(ostream &io,const MicroPolygon &m);

    void TransformToScreenSpace(Matrix4 &transform);

    bool Sample(float sx,float sy,float &z,Colour &colour,Opacity &opacity);
    void MicroPolygon::Rasterize(FrameBuffer &framebuffer);

  private:
    Point3 p[4];          // screen-space with Z in eye-space
    Colour colour;
    Opacity opacity;
};

#endif

