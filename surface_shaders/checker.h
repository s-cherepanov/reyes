#ifndef checker_h
#define checker_h
/*------------------------------------------------------
 * TITLE:  checker.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Plastic surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: checker.h,v $
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

class Checker: public Surface
{
  public:
    Checker(ParamList parameters)
      {Ka=0; Kd=1; Ks=1; roughness=10; darkcolour=Colour(0,0,0); sfreq=1; tfreq=1;
       if (parameters.Exists("Ka"))             Ka            =parameters.Float("Ka");
       if (parameters.Exists("Kd"))             Kd            =parameters.Float("Kd");
       if (parameters.Exists("Ks"))             Ks            =parameters.Float("Ks");
       if (parameters.Exists("roughness"))      roughness     =parameters.Float("roughness");
       if (parameters.Exists("darkcolour"))     darkcolour    =parameters.Vector("darkcolour");
       if (parameters.Exists("sfreq"))          sfreq         =parameters.Float("sfreq");
       if (parameters.Exists("tfreq"))          tfreq         =parameters.Float("tfreq");}

    void Dump()
      {cout << "Kd=" << Kd << " Ks=" << Ks << " roughness=" << roughness << " sfreq=" << sfreq << " tfreq=" << tfreq << "darkcolour=" << darkcolour << "\n";}

    void SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Surface texture co-ordinates
                       Vector3 N,       // Surface normal
                       Light *light,    // Light source
                       Colour &colour,Opacity &opacity)
      {int ss,tt;
       ss=(int)((s>=0)?(s*sfreq):(1-s*sfreq));
       tt=(int)((t>=0)?(t*tfreq):(1-t*tfreq));
       if (light)
       {
         if ((ss+tt)%2==0)
           colour=RiCurrent.shadingAttributes.colour*Kd*light->Diffuse(P,N)+Ks*light->Specular(P,N,1/roughness);
         else
           colour=darkcolour*Kd*light->Diffuse(P,N)+Ks*light->Specular(P,N,1/roughness);
       }
       else
       {
         colour=(((ss+tt)%2)==0)?darkcolour:RiCurrent.shadingAttributes.colour;
       }
       opacity=RiCurrent.shadingAttributes.opacity;
      }

  private:
    float sfreq,tfreq;
    Colour darkcolour;
    float Ka;
    float Kd;
    float Ks;
    float roughness;
};

#endif
