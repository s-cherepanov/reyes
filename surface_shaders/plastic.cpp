/*------------------------------------------------------
 * TITLE:  plastic.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Plastic surface shader
 * (C)     Copyright $Date: 1997/11/29 17:59:25 $
 * $Log: plastic.cpp,v $
 * Revision 1.1  1997/11/29 17:59:25  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:34:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "colour.h"
#include "vector3.h"
#include "point3.h"
#include "light.h"
#include "surface_shader.h"
#include "plastic.h"

Plastic::Plastic(ParamList parameters)
{
  Ka=0; Kd=1; Ks=1; roughness=10; specularcolour=Colour(1,1,1);

  if (parameters.Exists("Ka"))             Ka            =parameter.Float("Ka");
  if (parameters.Exists("Kd"))             Kd            =parameter.Float("Kd");
  if (parameters.Exists("Ks"))             Ks            =parameter.Float("Ks");
  if (parameters.Exists("roughness"))      roughness     =parameter.Float("roughness");
  if (parameters.Exists("specularcolour")) specularcolour=parameter.Float("specularcolour");

}

void Plastic::Dump()
{
  cout << "Ka=" << Ka << " Kd=" << Kd << " Ks=" << Ks << " roughness=" << roughness << " specularcolour=" << specularcolour << "\n";
}
