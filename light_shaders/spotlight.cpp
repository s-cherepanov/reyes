/*------------------------------------------------------
 * TITLE:  spotlight.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  NetBSD
 * DESC:   Useful REYES definitions
 * (C)     Copyright $Date: 1998/12/10 20:21:12 $
 * $Log: spotlight.cpp,v $
 * Revision 1.1  1998/12/10 20:21:12  ady
 * Initial revision
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
#include "spotlight.h"
#include "useful.h"
#include "ri.h"

//=======================================
// Constructor
//---------------------------------------
Spotlight::Spotlight(Point3 _from,Point3 _to,Colour _colour,float _intensity,
                     float _coneangle,float _conedeltaangle,float _beamdistribution)
{
  from=RiGlobal.tWorldToCamera*RiCurrent.transform*_from;
  to=RiGlobal.tWorldToCamera*RiCurrent.transform*_to;
  colour=_colour;
  intensity=_intensity;
  coneangle=_coneangle*0.5;
  conedeltaangle=_conedeltaangle*0.5;
  beamdistribution=_beamdistribution;
}

//=======================================
// Constructor
//---------------------------------------
Spotlight::Spotlight(ParamList parameters)
{
  int i;

  from=Point3(0,0,0);
  to=Point3(0,0,1);
  intensity=1;
  coneangle=0.5*30;
  conedeltaangle=0.5*5;
  beamdistribution=2;
  colour=Colour(1,1,1);

  if (parameters.Exists("from"))             from=parameters.Vector("from");
  if (parameters.Exists("to"))               to=parameters.Vector("to");
  if (parameters.Exists("intensity"))        intensity=parameters.Float("intensity");
  if (parameters.Exists("color"))            colour=parameters.Vector("color");
  if (parameters.Exists("coneangle"))        coneangle=0.5*parameters.Float("coneangle");
  if (parameters.Exists("conedeltaangle"))   conedeltaangle=0.5*parameters.Float("conedeltaangle");
  if (parameters.Exists("beamdistribution")) beamdistribution=parameters.Float("beamdistribution");

  from=RiGlobal.tWorldToCamera*RiCurrent.transform*from;
  to=RiGlobal.tWorldToCamera*RiCurrent.transform*to;
}

//=======================================
// LocalIntensity
//---------------------------------------
float Spotlight::LocalIntensity(Point3 p)
{
  Vector3 L,D;
  float dist2,d1,d2,i;

  d1=cos(DEGTORAD*coneangle);
  d2=cos(DEGTORAD*(1+coneangle+conedeltaangle));

  L=to-from;
  L.Normalise();
  D=p-from;
  dist2=D.x*D.x+D.y*D.y+D.z*D.z;
  D.Normalise();
  i=L*D;    /* Cos of angle between directions */

  if (i>=d1)
  {
    return intensity/dist2;
  }
  else
  {
    if (i>d2)
    {
      return intensity*(i-d2)/(dist2*(d1-d2));
    }
    else
    {
      return 0;
    }
  }
}

//=======================================
// Diffuse
//---------------------------------------
Colour Spotlight::Ambient()
{
  return Colour(0,0,0);
}

//=======================================
// Diffuse
//---------------------------------------
Colour Spotlight::Diffuse(Point3 P,Vector3 N)
{
  Vector3 L=to-from;
  float i;

  L.Normalise();
  i=N*L;
  i=MAX(-i,0);
  return (LocalIntensity(P)*i)*colour;
}

//=======================================
// Specular
//---------------------------------------
Colour Spotlight::Specular(Point3 P,Vector3 N,float shinyness)
{
  Vector3 L=to-from;
  Vector3 V=-P; // View vector
  float s,c;

  L.Normalise();
  V.Normalise();
  c=-N*L;
  s=(2*N*c+L)*V;
  s=MAX(s,0);
  return colour*(pow(s,shinyness)*LocalIntensity(P));
}

//=======================================
// Dump
//---------------------------------------
void Spotlight::Dump()
{
  cout << *this;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Spotlight &l)
{
  cout << "from: " << l.from << " to:" << l.to << " Colour: " << l.colour << " Inten.: " << l.intensity;
}
