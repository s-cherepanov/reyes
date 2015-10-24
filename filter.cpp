/*------------------------------------------------------
 * TITLE:  filter.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Filtering functions
 * (C)     Copyright $Date: 1998/12/10 20:06:36 $
 * $Log: filter.cpp,v $
 * Revision 1.1  1998/12/10 20:06:36  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "useful.h"

float RiBoxFilter(float x,float y,float xwidth,float ywidth)
{
  if ((ABS(x)<=xwidth)&&(ABS(y)<=ywidth)) return 1;
  return 0;
}

float RiGaussianFilter(float x,float y,float xwidth,float ywidth)
{
  return exp(-(x*x)/(2*xwidth*xwidth))*exp(-(y*y)/(2*ywidth*ywidth));
}
