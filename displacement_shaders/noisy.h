#ifndef noisy_h
#define noisy_h
/*------------------------------------------------------
 * TITLE:  noisy.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Noisy displacement shader
 * (C)     Copyright $Date: 1997/11/29 18:01:59 $
 * $Log: noisy.h,v $
 * Revision 1.1  1997/11/29 18:01:59  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "noise.h"
#include "vector3.h"
#include "point3.h"
#include "displacement.h"
#include "list.h"
#include "paramlist.h"

class Noisy: public Displacement
{
  public:
    Noisy(ParamList parameters)
    {scale=1.0; height=1.0;
     if (parameters.Exists("scale"))  scale=parameters.Float("scale");
     if (parameters.Exists("height")) height=parameters.Float("height");
    }

    void Dump()
      {cout << "scale=" << scale << " height=" << height << "\n";}

    float Value(Point3 P,        // Point in 3D eye/world/modelling space
                float s,float t, // Displacement texture co-ordinates
                Vector3 N)       // Surface normal
      {return noise1D(100+P.x/scale,100+P.y/scale,100+P.z/scale)*height;}

  private:
    float scale;
    float height;
};

#endif
