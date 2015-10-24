#ifndef matte_h
#define matte_h
/*------------------------------------------------------
 * TITLE:  matte.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Matte surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: matte.h,v $
 * Revision 1.1  1997/11/29 17:58:25  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "surface.h"

class Matte: public Surface
{
  public:
    Matte(ParamList parameters)
      {colour=Colour(1,1,1);
       if (parameters.Exists("colour")) parameters.Vector("colour");}

    Matte(Colour _colour)
      {colour=_colour;}

    void Dump()
      {cout << "Colour=" << colour << "\n";}

    void SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Surface texture co-ordinates
                       Vector3 N,       // Surface normal
                       Light *light,    // Light source
                       Colour &colour,Opacity &opacity)
      {colour=RiCurrent.shadingAttributes.colour;
       opacity=RiCurrent.shadingAttributes.opacity;}

  private:
    Colour colour;
};

#endif
