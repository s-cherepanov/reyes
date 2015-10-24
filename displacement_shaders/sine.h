#ifndef sine_h
#define sine_h
/*------------------------------------------------------
 * TITLE:  sine.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Sine displacement shader
 * (C)     Copyright $Date: 1997/11/29 18:01:59 $
 * $Log: sine.h,v $
 * Revision 1.1  1997/11/29 18:01:59  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "sine.h"
#include "vector3.h"
#include "point3.h"
#include "displacement_shader.h"
#include "list.h"
#include "parameter.h"
#include "useful.h"

class Sine: public DisplacementShader
{
  public:
    Sine(float _XFreq=1,float _YFreq=1,float _Height=1)
      {XFreq=_XFreq; YFreq=_YFreq; Height=_Height;}

    void Dump()
      {cout << "XFreq=" << XFreq << " YFreq=" << YFreq << " Height=" << Height << "\n";}

    float Displacement(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Displacement texture co-ordinates
                       Vector3 N)       // Surface normal
      {return Height*(sin(2*PI*s*XFreq)+sin(2*PI*t*YFreq));}

  private:
    float XFreq,YFreq;
    float Height;
};

#endif
