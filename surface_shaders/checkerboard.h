#ifndef checkerboard_h
#define checkerboard_h
/*------------------------------------------------------
 * TITLE:  checkerboard.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Plastic surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: checkerboard.h,v $
 * Revision 1.1  1997/11/29 17:58:25  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "surface.h"

class CheckerBoard: public Surface
{
  public:
    CheckerBoard(ParamList parameters)
      {Ka=0; Kd=1; Ks=1; roughness=10; Colour1=Colour(1,1,1); Colour2=Colour(0,0,0); TileWidth=1; TileHeight=1;
       if (parameters.Exists("Ka"))             Ka            =parameters.Float("Ka");
       if (parameters.Exists("Kd"))             Kd            =parameters.Float("Kd");
       if (parameters.Exists("Ks"))             Ks            =parameters.Float("Ks");
       if (parameters.Exists("roughness"))      roughness     =parameters.Float("roughness");
       if (parameters.Exists("Colour1"))        Colour1       =parameters.Vector("Colour1");
       if (parameters.Exists("Colour2"))        Colour2       =parameters.Vector("Colour2");
       if (parameters.Exists("TileWidth"))      TileWidth     =parameters.Float("TileWidth");
       if (parameters.Exists("TileHeight"))     TileHeight    =parameters.Float("TileHeight");}

    void Dump()
      {cout << "Kd=" << Kd << " Ks=" << Ks << " roughness=" << roughness << " TileWidth=" << TileWidth << " TileHeight=" << TileHeight << "Colour1=" << Colour1 << " Colour2=" << Colour2 << "\n";}

    Colour SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                         float s,float t, // Surface texture co-ordinates
                         Vector3 N,       // Surface normal
                         Light *light)    // Light source
      {int ss,tt;
       ss=(int)((s>=0)?(s/TileWidth):(1-s/TileWidth));
       tt=(int)((t>=0)?(t/TileHeight):(1-t/TileHeight));
       if (light)
       {
         if ((ss+tt)%2==0)
           return Colour1*Kd*light->Diffuse(P,N)+Ks*light->Specular(P,N,roughness);
         else
           return Colour2*Kd*light->Diffuse(P,N)+Ks*light->Specular(P,N,roughness);
       }
       else
       {
         return (((ss+tt)%2)==0)?Colour1:Colour2;
       }
      }

  private:
    float TileWidth,TileHeight;
    Colour Colour1,Colour2;
    float Ka;
    float Kd;
    float Ks;
    float roughness;
};

#endif
