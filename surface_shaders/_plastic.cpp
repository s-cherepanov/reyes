/*------------------------------------------------------
 * TITLE:  _plastic.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Plastic surface shader
 * (C)     Copyright $Date: 1997/11/29 17:59:25 $
 * $Log: _plastic.cpp,v $
 * Revision 1.1  1997/11/29 17:59:25  ady
 * Initial revision
 *
 * Revision 1.1  1997/03/01 20:35:04  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdlib.h>

// Reyes includes
#include "colour.h"
#include "vector3.h"
#include "point3.h"
#include "light.h"
#include "surface_shader.h"
#include "plastic.h"

int main(int argc,char *argv[])
{
  SurfaceShader *ss=new Plastic(1,1,20);

  ss->Dump();

  return EXIT_SUCCESS;
}
