#ifndef colour_h
#define colour_h
/*------------------------------------------------------
 * TITLE:  colour.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Colour class
 * (C)     Copyright $Date: 1998/12/10 20:14:02 $
 * $Log: colour.h,v $
 * Revision 1.2  1998/12/10 20:14:02  ady
 * Added new operations on colours
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "vector3.h"

class Colour
{
  public:
    float r,g,b;

    Colour(const Vector3 &v)
      {r=v.x; g=v.y; b=v.z;}

    Colour(float _r=0,float _g=0,float _b=0)
      {r=_r; g=_g; b=_b;};

    // Arithmetic operations
    friend Colour operator+(Colour a,Colour b);
    friend Colour operator-(Colour a,Colour b);
    friend Colour operator*(Colour a,Colour b);
    friend Colour operator*(float s,Colour c);
    friend Colour operator*(Colour c,float s);
    friend Colour operator/(Colour c,float s);

    Colour operator+=(Colour c){r+=c.r; g+=c.g; b+=c.b; return *this;}
    Colour operator*=(Colour c){r*=c.r; g*=c.g; b*=c.b; return *this;}
 
    friend ostream &operator<<(ostream &io,const Colour &colour);

};

#endif
