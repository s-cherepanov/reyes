#ifndef marble_h
#define marble_h
/*------------------------------------------------------
 * TITLE:  marble.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Marble surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: marble.h,v $
 * Revision 1.1  1997/11/29 17:58:25  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "surface.h"
#include "ri.h"

class Marble: public Surface
{
  public:
    Marble(Colour _darkcolour=Colour(0,0,0),float _Ka=0.0,float _Kd=1.0,float _Ks=1.0,float _roughness=10.0,float _freq=1,float _freqscale=2,float _powerscale=0.5,float _nooffreqs=2)
      {darkcolour=_darkcolour; Ka=_Ka; Kd=_Kd; Ks=_Ks; roughness=_roughness;
       freq=_freq; freqscale=_freqscale; powerscale=_powerscale; nooffreqs=_nooffreqs;}

    Marble(ParamList parameters)
    {Ka=0; Kd=1; Ks=1; roughness=10; darkcolour=Colour(0,0,0);
     freq=1; freqscale=2; powerscale=0.5; nooffreqs=2;
     if (parameters.Exists("Ka"))             Ka            =parameters.Float("Ka");
     if (parameters.Exists("Kd"))             Kd            =parameters.Float("Kd");
     if (parameters.Exists("Ks"))             Ks            =parameters.Float("Ks");
     if (parameters.Exists("freq"))           freq          =parameters.Float("freq");
     if (parameters.Exists("freqscale"))      freqscale     =parameters.Float("freq");
     if (parameters.Exists("powerscale"))     powerscale    =parameters.Float("powerscale");
     if (parameters.Exists("roughness"))      roughness     =parameters.Float("roughness");
     if (parameters.Exists("darkcolour"))     darkcolour=parameters.Vector("darkcolour");}

    void Dump()
      {cout << "darkcolour=" << darkcolour << " Ka=" << Ka << " Kd=" << Kd << " Ks=" << Ks << " freq=" << freq << " roughness=" << roughness << "\n";}

    void SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Surface texture co-ordinates
                       Vector3 N,       // Surface normal
                       Light *light,    // Light source
                       Colour &colour,Opacity &opacity)
      {float v=0; Colour c; int f;
       float freq2=freq,power2=1;
       for(f=0;f<nooffreqs;f++)
       {
         P+=power2*(noise3D(P.x*freq2,P.y*freq2,P.z*freq2)-Point3(0.5,0.5,0.5));
         freq2*=freqscale;
         power2*=powerscale;
       }
       v=CLAMP(noise1D(P.x,P.y,P.z),0,1);
       c=(v*RiCurrent.shadingAttributes.colour)+((1-v)*darkcolour);
       colour=c*Kd*light->Diffuse(P,N)+Ks*light->Specular(P,N,roughness);
       opacity=RiCurrent.shadingAttributes.opacity;}

  private:
    Colour darkcolour;
    float freq;
    float freqscale;
    float powerscale;
    float nooffreqs;
    float Ka;
    float Kd;
    float Ks;
    float roughness;
};

#endif
