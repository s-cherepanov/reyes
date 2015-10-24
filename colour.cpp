/*------------------------------------------------------
 * TITLE:  colour.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Colour class
 * (C)     Copyright $Date: 1997/11/16 14:15:10 $
 * $Log: colour.cpp,v $
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "colour.h"

Colour operator+(Colour a,Colour b)
{
  return Colour(a.r+b.r,a.g+b.g,a.b+b.b);
}

Colour operator-(Colour a,Colour b)
{
  return Colour(a.r-b.r,a.g-b.g,a.b-b.b);
}

Colour operator*(Colour a,Colour b)
{
  return Colour(a.r*b.r,a.g*b.g,a.b*b.b);
}

Colour operator*(float s,Colour c)
{
  return Colour(s*c.r,s*c.g,s*c.b);
}

Colour operator*(Colour c,float s)
{
  return Colour(s*c.r,s*c.g,s*c.b);
}

Colour operator/(Colour c,float s)
{
  return Colour(c.r/s,c.g/s,c.b/s);
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================
ostream &operator<<(ostream &io,const Colour &colour)
{
  io << "(" << colour.r << " " << colour.g << " " << colour.b << ")";
  return io;
}
