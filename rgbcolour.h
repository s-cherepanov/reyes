#ifndef rgbcolour_h
#define rgbcolour_h
/*------------------------------------------------------
 * TITLE:  rgbcolour.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   RGB colour class
 * (C)     Copyright $Date: 1998/12/10 20:17:43 $
 * $Log: rgbcolour.h,v $
 * Revision 1.2  1998/12/10 20:17:43  ady
 * Includes reyes now
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include "iostream.h"

// Reyes includes
#include "colour.h"

class RGBColour
{
  public:
    RGBColour(int red=0,int green=0,int blue=0): // Constructor
      r(red),g(green),b(blue){}

    // Stream input/output
    friend ostream &operator<<(ostream &io,const RGBColour &c);  

    unsigned char r,g,b;
};

#endif
