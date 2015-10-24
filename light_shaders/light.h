#ifndef light_h
#define light_h
/*------------------------------------------------------
 * TITLE:  light.h
 * ENVIR:  RiscBSD
 * DESC:   Header for shaders
 * (C)     Copyright $Date: 1998/12/10 20:21:33 $
 * $Log: light.h,v $
 * Revision 1.2  1998/12/10 20:21:33  ady
 * Make Ri style
 *
 * Revision 1.1  1997/11/29 17:56:41  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "paramlist.h"
#include "noise.h"
#include "colour.h"
#include "point3.h"
#include "texturemap.h"
#include "matrix4.h"

class Light
{
  public:
    virtual Colour Ambient()
      {cout << "Ambient():No such light shader\n"; return Colour(0,0,0);}
    virtual Colour Diffuse(Point3 P,Vector3 N)
      {cout << "Diffuse():No such light shader\n"; return Colour(0,0,0);}
    virtual Colour Specular(Point3 P,Vector3 N,float shinyness)
      {cout << "Specular():No such light shader\n"; return Colour(0,0,0);}
    virtual void Dump()
      {cout << "Dump():No such light shader\n";}
};

#endif
