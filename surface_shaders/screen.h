#ifndef screen_h
#define screen_h
/*------------------------------------------------------
 * TITLE:  screen.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Plastic surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: screen.h,v $
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

class Screen: public Surface
{
  public:
    Screen(ParamList parameters)
      {Ka=0; Kd=1; Ks=1; roughness=0.1; fuzz=0;
       sfreq=1; tfreq=1; sdensity=0.5; tdensity=0.5;
       if (parameters.Exists("Ka"))             Ka            =parameters.Float("Ka");
       if (parameters.Exists("Kd"))             Kd            =parameters.Float("Kd");
       if (parameters.Exists("Ks"))             Ks            =parameters.Float("Ks");
       if (parameters.Exists("roughness"))      roughness     =parameters.Float("roughness");
       if (parameters.Exists("fuzz"))           fuzz          =parameters.Float("fuzz");
       if (parameters.Exists("sfreq"))          sfreq         =parameters.Float("sfreq");
       if (parameters.Exists("tfreq"))          tfreq         =parameters.Float("tfreq");
       if (parameters.Exists("sdensity"))       sdensity      =parameters.Float("sdensity");
       if (parameters.Exists("tdensity"))       tdensity      =parameters.Float("tdensity");}

    void Dump()
      {cout << "Kd=" << Kd << " Ks=" << Ks << " roughness=" << roughness << " sfreq=" << sfreq << " tfreq=" << tfreq << "\n";}

    void SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Surface texture co-ordinates
                       Vector3 N,       // Surface normal
                       Light *light,    // Light source
                       Colour &colour,Opacity &opacity)
      {float Oi;
       colour=RiCurrent.shadingAttributes.colour*Kd*light->Diffuse(P,N)+Ks*light->Specular(P,N,1/roughness);
       Oi=1-smoothstep(sdensity-fuzz/2,sdensity+fuzz/2,ABS(mod(s*sfreq,1)))*
            smoothstep(tdensity-fuzz/2,tdensity+fuzz/2,ABS(mod(t*tfreq,1)));
       opacity=Opacity(Oi,Oi,Oi);
      }

  private:
    float sfreq,tfreq;
    float fuzz;
    float sdensity,tdensity;
    float Ka;
    float Kd;
    float Ks;
    float roughness;
};

#endif
