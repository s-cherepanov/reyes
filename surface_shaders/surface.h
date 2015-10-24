#ifndef surface_h
#define surface_h
/*------------------------------------------------------
 * TITLE:  surface.h
 * ENVIR:  RiscBSD
 * DESC:   Header for shaders
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: surface.h,v $
 * Revision 1.1  1997/11/29 17:58:25  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "paramlist.h"
#include "light.h"
#include "noise.h"
#include "smath.h"
#include "texturemap.h"
#include "opacity.h"

class Surface
{
  public:
    virtual void SurfaceColour(Point3 P,float s,float t,Vector3 N,Light *light,Colour &colour,Opacity &opacity)
      {cerr << "No such surface\n";}

    virtual void Dump()
      {cerr << "No such surface\n";}
};

#endif
