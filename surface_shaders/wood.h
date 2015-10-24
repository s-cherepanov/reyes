#ifndef wood_h
#define wood_h
/*------------------------------------------------------
 * TITLE:  wood.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Wood surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: wood.h,v $
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

class Wood: public Surface
{
  public:
    Wood(ParamList parameters)
    {Ka=0.2; Kd=0.4; Ks=0.6; roughness=0.1; ringscale=10;
     lightwood=Colour(0.3,0.12,0.03); darkwood=Colour(0.05,0.01,0.005);
     if (parameters.Exists("Ka"))             Ka            =parameters.Float("Ka");
     if (parameters.Exists("Kd"))             Kd            =parameters.Float("Kd");
     if (parameters.Exists("Ks"))             Ks            =parameters.Float("Ks");
     if (parameters.Exists("roughness"))      roughness     =parameters.Float("roughness");
     if (parameters.Exists("lightwood"))      lightwood     =parameters.Vector("lightwood");
     if (parameters.Exists("darkwood"))       darkwood      =parameters.Vector("darkwood");
     if (parameters.Exists("ringscale"))      ringscale     =parameters.Float("ringscale");}

    void Dump()
      {cout << "ringscale=" << ringscale << " Ka=" << Ka << " Kd=" << Kd << " Ks=" << Ks << "roughness=" << roughness << " lightwood=" << lightwood << " darkwood=" << darkwood << "\n";}

    void SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Surface texture co-ordinates
                       Vector3 N,       // Surface normal
                       Light *light,    // Light source
                       Colour &Ci,Opacity &Oi)
      {Colour c;
       Point3 PP=P;
       float r,y,z;
       PP+=noise3D(P.x,P.y,P.z);

       /* compute radial distance r from P to axis of `tree' */
       y=PP.y;
       z=PP.z;
       r=sqrt(y*y+z*z);

       /* map radial distance r into ring position [0,1] */
       r*=ringscale;
       r+=ABS(noise1D(r,0,0));
       r=r-(int)r;

       /* use ring position r to select wood colour */
       r=smoothstep(0,0.8,r)-smoothstep(0.83,1.0,r);
       Ci=mix(lightwood,darkwood,r);
       Oi=RiCurrent.shadingAttributes.opacity;
       Ci=Oi*Ci*(Kd*light->Diffuse(P,N))+
	  (0.3*r+0.7)*Ks*light->Specular(P,N,1/(0.001+roughness));
      }

  private:
    Colour lightwood;
    Colour darkwood;
    float ringscale;
    float Ka,Kd,Ks;
    float roughness;
};

#endif
