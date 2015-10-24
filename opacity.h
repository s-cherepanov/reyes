#ifndef opacity_h
#define opacity_h
/*------------------------------------------------------
 * TITLE:  opacity.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Opacity class
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: opacity.h,v $
 * Revision 1.2  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/10 20:16:50  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "colour.h"

class Opacity: public Colour
{
  public:
    Opacity(float _r=1,float _g=1,float _b=1)
      {r=_r; g=_g; b=_b;}
};

#endif
