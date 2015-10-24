#ifndef threads_h
#define threads_h
/*------------------------------------------------------
 * TITLE:  threads.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Threads displacement shader
 * (C)     Copyright $Date: 1998/12/10 20:22:26 $
 * $Log: threads.h,v $
 * Revision 1.1  1998/12/10 20:22:26  ady
 * Initial revision
 *
 * Revision 1.1  1997/11/29 18:01:59  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "colour.h"
#include "vector3.h"
#include "point3.h"
#include "displacement.h"
#include "list.h"
#include "paramlist.h"
#include "noise.h"

class Threads: public Displacement
{
  public:
    Threads(ParamList parameters)
      {Km=1; frequency=5; phase=0; offset=0; dampzone=0.05;
       if (parameters.Exists("Km") )        Km=parameters.Float("Km");
       if (parameters.Exists("frequency"))  frequency=parameters.Float("frequency");
       if (parameters.Exists("phase"))      phase=parameters.Float("phase");
       if (parameters.Exists("offset"))     offset=parameters.Float("offset");
       if (parameters.Exists("dampzone"))   dampzone=parameters.Float("dampzone");}

    void Dump()
      {cout << "\n";}

    float Value(Point3 P,        // Point in 3D eye/world/modelling space
                float s,float t, // Displacement texture co-ordinates
                Vector3 N)       // Surface normal
      {float magnitude;
       magnitude=(sin(M_PI*2*(t*frequency+s+phase))+offset)*Km;
       if (t>(1-dampzone))
	 magnitude*=(1-t)/dampzone;
       else if (t<dampzone)
	 magnitude*=t/dampzone;
       return magnitude;
      }

  private:
    float Km;
    float frequency;
    float phase;
    float offset;
    float dampzone;
};

#endif
