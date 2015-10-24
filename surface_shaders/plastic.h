#ifndef plastic_h
#define plastic_h
/*------------------------------------------------------
 * TITLE:  plastic.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Plastic surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: plastic.h,v $
 * Revision 1.1  1997/11/29 17:58:25  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "surface.h"
#include "ri.h"

class Plastic: public Surface
{
  public:
    Plastic(ParamList parameters)
    {Ka=0; Kd=1; Ks=1; roughness=10; specularcolour=Colour(1,1,1);
     if (parameters.Exists("Ka"))             Ka            =parameters.Float("Ka");
     if (parameters.Exists("Kd"))             Kd            =parameters.Float("Kd");
     if (parameters.Exists("Ks"))             Ks            =parameters.Float("Ks");
     if (parameters.Exists("roughness"))      roughness     =parameters.Float("roughness");
     if (parameters.Exists("specularcolour")) specularcolour=parameters.Vector("specularcolour");}

    Plastic(Colour _specularcolour=Colour(1,1,1),float _Ka=0.0,float _Kd=0.8,float _Ks=1.0,float _roughness=30.0)
      {Ka=_Ka; Kd=_Kd; Ks=_Ks; roughness=_roughness; specularcolour=_specularcolour;}

    void Dump()
      {cout << "specularcolour=" << specularcolour << " Kd=" << Kd << " Ks=" << Ks << " roughness=" << roughness << "\n";}

    void SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Surface texture co-ordinates
                       Vector3 N,       // Surface normal
                       Light *light,    // Light source
                       Colour &colour,Opacity &opacity)
      {colour=RiCurrent.shadingAttributes.colour*Kd*light->Diffuse(P,N)+
              Ks*specularcolour*light->Specular(P,N,1/(0.001+roughness));
       opacity=RiCurrent.shadingAttributes.opacity;}

  private:
    Colour specularcolour;
    float Ka;
    float Kd;
    float Ks;
    float roughness;
};

#endif

