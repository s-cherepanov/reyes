/*------------------------------------------------------
 * TITLE:  displacement_shaders.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Create new displacement shaders
 * (C)     Copyright $Date$
 * $Log$
 *-----------------------------------------------------*/

// C includes
#include <stdlib.h>

// C++ includes
#include "iostream.h"

// Reyes includes
#include "displacement_shaders.h"
#include "displacement_shader.h"

DisplacementShader *NewDisplacementShader(char *name,List<Parameter> *list)
{
  DisplacementShader *ss=NULL;

  if (!ss)
  {
    cerr << "Displacement shader `" << name << "' does not exist\n";
    exit(EXIT_FAILURE);
  }
  ss->Dump();
  return ss;
}
