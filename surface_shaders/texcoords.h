#ifndef texcoords_h
#define texcoords_h
/*------------------------------------------------------
 * TITLE:  texcoords.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Texcoords surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: texcoords.h,v $
 * Revision 1.1  1997/11/29 17:58:25  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "surface.h"

class TexCoords: public SurfaceShader
{
  public:
    TexCoords()
      {}

    void Dump()
      {}

    Colour SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                         float s,float t, // Surface texture co-ordinates
                         Vector3 N,       // Surface normal
                         Light *light)    // Light source
      {Colour c; c=Colour(s,t,0); return c;}

  private:
};

#endif
