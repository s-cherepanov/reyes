#ifndef imagemap_h
#define imagemap_h
/*------------------------------------------------------
 * TITLE:  imagemap.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Imagemap surface shader
 * (C)     Copyright $Date: 1997/11/29 17:58:25 $
 * $Log: imagemap.h,v $
 * Revision 1.1  1997/11/29 17:58:25  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "surface.h"

class ImageMap: public SurfaceShader
{
  public:
    ImageMap(float _Kd=1.0,float _Ks=1.0,float _Shinyness=10.0)
      {Kd=_Kd; Ks=_Ks; Shinyness=_Shinyness;}

    void Dump()
      {cout << "ImageMap{Kd=" << Kd << " Ks=" << Ks << " Shinyness=" << Shinyness << " Width=" << width << " Height=" << height << "}\n";}

    Colour SurfaceColour(Point3 P,        // Point in 3D eye/world/modelling space
                         floa at,float t, // Surface texture co-ordinates
                         Vector3 N,       // Surface normal
                         Light *light)    // Light source
      {Colour c;
       int x=(int)((image->Width()-1)*s/width);
       int y=(int)((image->Height()-1)*t/height);
       c=image->ReadColour(x%image->Width(),y%image->Height())*Kd*light->Diffuse(P,N)+Ks*light->Specular(P,N,Shinyness); return c;}

  private:
    Image *image;
    float width;
    float height;
    float Kd;
    float Ks;
    float Shinyness;
};

#endif
