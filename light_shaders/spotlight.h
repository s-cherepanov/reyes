#ifndef spotlight_h
#define spotlight_h
/*------------------------------------------------------
 * TITLE:  spotlight.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  NetBSD
 * DESC:   Light source class
 * (C)     Copyright $Date: 1998/12/10 20:21:33 $
 * $Log: spotlight.h,v $
 * Revision 1.1  1998/12/10 20:21:33  ady
 * Initial revision
 *
 * Revision 1.1  1997/11/29 17:56:18  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "light.h"

class Spotlight: public Light
{
  public:
    Spotlight(ParamList parameters);
    Spotlight(Point3 _from=Point3(0,0,0),Point3 _to=Point3(0,0,1),Colour _color=Colour(1,1,1),float _intensity=1,float _coneangle=30,float _conedeltaangle=5,float _beamdistribution=2.0);

    Colour Ambient();
    Colour Diffuse(Point3 P,Vector3 N);
    Colour Specular(Point3 P,Vector3 N,float shinyness);

    void Dump();

    friend ostream &operator<<(ostream &io,const Spotlight &l);

  private:
    float LocalIntensity(Point3 p);

    Point3 from,to;
    Colour colour;
    float intensity;
    float coneangle;
    float conedeltaangle;
    float beamdistribution;
};

#endif
