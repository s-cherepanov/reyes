#ifndef smath_h
#define smath_h
/*------------------------------------------------------
 * TITLE:  smath.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Mathematical shading functions
 * (C)     Copyright $Date: 1998/12/10 20:18:05 $
 * $Log: smath.h,v $
 * Revision 1.1  1998/12/10 20:18:05  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

#include "useful.h"

template <class T>inline T min(T a,T b)
  {return (a<b)?a:b;}

template <class T>inline T max(T a,T b)
  {return (a>b)?a:b;}

template <class T>inline T mix(T a,T b,float t)
  {return a*(1-t)+b*t;}

inline float mod(float a,float b)
  {return ABS(a-b*((int)(0.5+a/b)));}

inline float abs(float x)
  {return (x>=0)?(x):-(x);}

inline float step(float min,float val)
  {return (val<0)?0:1;}

inline float smoothstep(float min,float max,float val)
  {return (val<min)?0:((val>=max)?1:(val-min)/(max-min));}

#endif
