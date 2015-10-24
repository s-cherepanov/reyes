/*------------------------------------------------------
 * TITLE:  distantlight.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  NetBSD
 * DESC:   Useful REYES definitions
 * (C)     Copyright $Date: 1998/12/10 20:21:12 $
 * $Log: distantlight.cpp,v $
 * Revision 1.2  1998/12/10 20:21:12  ady
 * Made Ri style
 *
 * Revision 1.1  1997/11/29 17:56:01  ady
 * Initial revision
 *
 * Revision 1.2  1997/11/21 20:14:41  ady
 * meaningless change
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "distantlight.h"
#include "useful.h"
#include "ri.h"

//=======================================
// Constructor
//---------------------------------------
Distantlight::Distantlight(Point3 _from,Point3 _to,Colour _colour,float _intensity)
{
  L=RiGlobal.tWorldToCamera*RiCurrent.transform*_to-RiGlobal.tWorldToCamera*RiCurrent.transform*_from;
  L.Normalise();
  colour=_colour;
  intensity=_intensity;
}

//=======================================
// Constructor
//---------------------------------------
Distantlight::Distantlight(ParamList parameters)
{
  int i;
  Vector3 from=Point3(0,0,0);
  Vector3 to=Point3(0,0,1);

  if (parameters.Exists("from"))      from=parameters.Vector("from");
  if (parameters.Exists("to"))        to=parameters.Vector("to");
  if (parameters.Exists("intensity")) intensity=parameters.Float("intensity");
  if (parameters.Exists("color"))     colour=parameters.Vector("color");
  L=to-from;
  L=RiGlobal.tWorldToCamera*RiCurrent.transform*L;
  L.Normalise();
}

//=======================================
// Diffuse
//---------------------------------------
Colour Distantlight::Ambient()
{
  return Colour(0,0,0);
}

//=======================================
// Diffuse
//---------------------------------------
Colour Distantlight::Diffuse(Point3 P,Vector3 N)
{
  float i=N*L;
  i=MAX(-i,0);
  return (intensity*i)*colour;
}

//=======================================
// Specular
//---------------------------------------
Colour Distantlight::Specular(Point3 P,Vector3 N,float shinyness)
{
  Vector3 V=-P; // View vector
  float s,c;

  V.Normalise();
  c=-N*L;
  s=(2*N*c+L)*V;
  s=MAX(s,0);
  return colour*(pow(s,shinyness)*intensity);
}

//=======================================
// Dump
//---------------------------------------
void Distantlight::Dump()
{
  cout << *this;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Distantlight &l)
{
  cout << "Dir: " << l.L << " Colour: " << l.colour << " Inten.: " << l.intensity;
}


