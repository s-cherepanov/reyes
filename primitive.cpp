/*------------------------------------------------------
 * TITLE:  primitive.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Primitive class (container of sphere,polygon,patch,etc...)
 * (C)     Copyright $Date: 1998/12/10 20:10:28 $
 * $Log: primitive.cpp,v $
 * Revision 1.3  1998/12/10 20:10:28  ady
 * Changed estimation of microgrid size
 *
 * Revision 1.2  1997/11/29 18:02:41  ady
 * New dicing and estimate gridsize method
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "ri.h"
#include "primitive.h"
#include "useful.h"

// C++ includes
#include "iostream.h"
#include "iomanip.h"

//=======================================
// EstimateGridSize
//---------------------------------------
void Primitive::EstimateGridSize(float xscale,float yscale,int &us,int &vs)
{
  MicroGrid microgrid;
  float zmin,zmax,maxusize,maxvsize;

  DoDice(microgrid,10,10);
  microgrid.Statistics(zmin,zmax,maxusize,maxvsize);
  us=MAX(4,(int)(10*xscale*maxusize/(RiCurrent.shadingAttributes.shadingRate*RiGlobal.display.xSamplingRate)));
  vs=MAX(4,(int)(10*yscale*maxvsize/(RiCurrent.shadingAttributes.shadingRate*RiGlobal.display.ySamplingRate)));
  // Make size a power of 2
  us=1<<(int)(logn(2,us));
  vs=1<<(int)(logn(2,us));
  us=MAX(us,vs);
  vs=MAX(us,vs);
}

//=======================================
// Diceable
//---------------------------------------
bool Primitive::Diceable(float xscale,float yscale)
{
  int us,vs;

  EstimateGridSize(xscale,yscale,us,vs);
  if (us*vs<=RiGlobal.options.maxMicroGridSize)
    return TRUE;
  else
    return FALSE;
}

//=======================================
// Dice
//---------------------------------------
void Primitive::Dice(MicroGrid &microgrid,float xscale,float yscale)
{
  int us,vs;

  EstimateGridSize(xscale,yscale,us,vs);
  DoDice(microgrid,us,vs);
}
