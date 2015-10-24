#ifndef fractal_h
#define fractal_h
/*------------------------------------------------------
 * TITLE:  fractal.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Fractal displacement shader
 * (C)     Copyright $Date: 1998/12/10 20:22:26 $
 * $Log: fractal.h,v $
 * Revision 1.2  1998/12/10 20:22:26  ady
 * Made Ri style
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

class Fractal: public Displacement
{
  public:
    Fractal(ParamList parameters)
      {freq=1; height=1; freqmult=2; powermult=0.5; nooffreqs=3; power=1;
       if (parameters.Exists("freq") )     freq=parameters.Float("freq");
       if (parameters.Exists("height"))    height=parameters.Float("height");
       if (parameters.Exists("freqmult"))  freqmult=parameters.Float("freqmult");
       if (parameters.Exists("powermult")) powermult=parameters.Float("powermult");
       if (parameters.Exists("nooffreqs")) nooffreqs=(int)parameters.Float("nooffreqs");
       if (parameters.Exists("power"))     power=parameters.Float("power");}

    void Dump()
      {cout << "Freq=" << freq << " Height=" << height << "\n";}

    float Value(Point3 P,        // Point in 3D eye/world/modelling space
                float s,float t, // Displacement texture co-ordinates
                Vector3 N)       // Surface normal
      {float n,freq2=freq,height2=0.5,h;
       int i;

       n=0;
       for(i=0;i<nooffreqs;i++)
       {
         n+=height2*noise1D(P.x*freq2,P.y*freq2,P.z*freq2);
	 freq2*=freqmult;
         height2*=powermult;
       }
       return height*pow(n,power);}

  private:
    float freq,height,freqmult,powermult;
    int nooffreqs;
    float power;
};

#endif
