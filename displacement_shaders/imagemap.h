#ifndef imagemap_h
#define imagemap_h
/*------------------------------------------------------
 * TITLE:  imagemap.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Imagemap surface shader
 * (C)     Copyright $Date: 1997/11/29 18:01:59 $
 * $Log: imagemap.h,v $
 * Revision 1.1  1997/11/29 18:01:59  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:56  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "colour.h"
#include "vector3.h"
#include "point3.h"
#include "displacement_shader.h"
#include "list.h"
#include "parameter.h"
#include "image.h"

class ImageMap: public DisplacementShader
{
  public:
    ImageMap(int _width=1,int _height=1)
      {width=1; height=1;}

    void Dump()
      {}

    float Displacement(Point3 P,        // Point in 3D eye/world/modelling space
                       float s,float t, // Displacement texture co-ordinates
                       Vector3 N)       // Surface normal
      {int x=(int)((image->Width()-1)*s/width);
       int y=(int)((image->Height()-1)*t/height);
       return scale*image->ReadGrey(x%image->Width(),y%image->Height());}

  private:
    Image *image;
    float width;
    float height;
    float scale;
};

#endif
